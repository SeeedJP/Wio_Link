#!/usr/bin/env python3

import os
import sqlite3 as lite
import hashlib
import sys


def get_db_path() -> str:
    db_dir = os.environ.get('WIO_LINK_DATABASE_FOLDER')
    if db_dir:
        return os.path.join(db_dir, 'database.db')
    # fallback to local path inside workdir
    return 'database.db'


def ensure_db_exists(db_path: str) -> None:
    # server.py will copy a default database into WIO_LINK_DATABASE_FOLDER on startup if it doesn't exist,
    # but for first-run initialization we need the file to be present now.
    # If it doesn't exist yet but a template exists in CWD, copy it; otherwise just create empty tables error out gracefully.
    if os.path.exists(db_path):
        return
    template = 'database.db'
    if os.path.exists(template):
        try:
            import shutil
            os.makedirs(os.path.dirname(db_path), exist_ok=True)
            shutil.copyfile(template, db_path)
            return
        except Exception as e:
            print(f"[init_user] Failed to copy default database to {db_path}: {e}")
            # fall through to create directory and let server handle
    # Ensure directory exists to allow server.py to copy on startup
    try:
        os.makedirs(os.path.dirname(db_path) or '.', exist_ok=True)
    except Exception:
        pass


def md5(s: str) -> str:
    return hashlib.md5(s.encode()).hexdigest()


def main() -> int:
    email = os.environ.get('WIO_LINK_DEFAULT_EMAIL', 'admin@local.com')
    password = os.environ.get('WIO_LINK_DEFAULT_PASSWORD', 'password')

    db_path = get_db_path()
    ensure_db_exists(db_path)

    if not os.path.exists(db_path):
        print(f"[init_user] Database not found at {db_path}. It will be created/seeded by server.py on first boot. Skipping user init.")
        return 0

    try:
        con = lite.connect(db_path)
        con.row_factory = lite.Row
        cur = con.cursor()
    except Exception as e:
        print(f"[init_user] Unable to open database {db_path}: {e}")
        return 1

    try:
        # Ensure users table exists
        cur.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='users'")
        if cur.fetchone() is None:
            print("[init_user] 'users' table not found. Skipping user init.")
            return 0

        # If user already exists, exit quietly
        cur.execute("SELECT user_id, email, token FROM users WHERE email=?", (email,))
        row = cur.fetchone()
        if row is not None:
            print(f"[init_user] User already exists: {row['email']}")
            return 0

        # Insert new user consistent with handlers.py (MD5 pwd, token format is an opaque string)
        # Replicate token generation approach from handlers.py: base64url(HMAC_SHA256(secret, base64url(header) + '.' + base64url(payload)))
        # We cannot import handlers here reliably, so generate a simple deterministic token compatible with the schema.
        token = md5(email + ':' + password)

        # Generate user_id similar to gen_uuid_without_dash
        import uuid
        user_id = str(uuid.uuid1()).replace('-', '')

        cur.execute(
            "INSERT INTO users(user_id,email,pwd,token,created_at) VALUES(?,?,?,?,datetime('now'))",
            (user_id, email, md5(password), token),
        )
        con.commit()
        print(f"[init_user] Created default user '{email}' with password '{password}'.")
        print(f"[init_user] Token: {token}")
        return 0
    except Exception as e:
        print(f"[init_user] Error during user initialization: {e}")
        return 1
    finally:
        try:
            cur.close()
        except Exception:
            pass
        try:
            con.close()
        except Exception:
            pass


if __name__ == "__main__":
    sys.exit(main())


