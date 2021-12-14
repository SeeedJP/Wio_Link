#!/usr/bin/env python3

import os
import sqlite3 as lite
from contextlib import contextmanager
import uuid
import hashlib
import base64
import hmac


class Database:

    def __init__(self):
        database_path = os.environ.get('WIO_LINK_DATABASE_FOLDER')
        if database_path:
            database_path = os.path.join(database_path, 'database.db')
        else:
            database_path = 'database.db'

        if not os.path.exists(database_path):
            raise Exception(f"Cannot found {database_path} file.")

        self.__con = lite.connect(database_path)
        self.__con.row_factory = lite.Row

    def __del__(self):
        self.__con.close()


    def get_stats(self) -> dict:
        with _managed_cursor(self.__con.cursor()) as cur:
            cur.execute("SELECT COUNT(user_id) FROM users")
            user_count = cur.fetchone()[0]
            cur.execute("SELECT COUNT(node_id) FROM nodes")
            node_count = cur.fetchone()[0]
        
        return { "user_count": user_count, "node_count": node_count };


    def get_user_list(self, email: str = None) -> list:
        with _managed_cursor(self.__con.cursor()) as cur:
            if email is None:
                cur.execute("SELECT user_id,email,created_at,max_node_count FROM users")
            elif email is not None:
                cur.execute("SELECT user_id,email,created_at,max_node_count FROM users WHERE email=?", (email,))
            rows = cur.fetchall()

        return rows

    def add_user(self, email: str, password: str, max_node_count: int = None) -> str:
        with _managed_cursor(self.__con.cursor()) as cur:
            cur.execute("SELECT COUNT(user_id) FROM users WHERE email=?", (email,))
            if cur.fetchone()[0] >= 1:
                raise Exception("That email is already registered.")

        user_id = _generate_user_id()
        hash_password = _hash_password(password)
        token = _generate_token(email)

        with _managed_cursor(self.__con.cursor()) as cur:
            cur.execute("INSERT INTO users(user_id,email,pwd,token,max_node_count,created_at) VALUES(?,?,?,?,?,datetime('now'))", (user_id, email, hash_password, token, max_node_count))
        self.__con.commit()

        return user_id

    def remove_user(self, email: str = None) -> str:
        if email is None:
            raise Exception("Conditions are required.")

        with _managed_cursor(self.__con.cursor()) as cur:
            cur.execute("DELETE FROM builds WHERE node_id IN (SELECT n.node_id FROM nodes n INNER JOIN users u ON n.user_id = u.user_id WHERE u.email=?)", (email,))
            cur.execute("DELETE FROM resources WHERE node_id IN (SELECT n.node_id FROM nodes n INNER JOIN users u ON n.user_id = u.user_id WHERE u.email=?)", (email,))
            cur.execute("DELETE FROM nodes WHERE user_id IN (SELECT u.user_id FROM users u WHERE u.email=?)", (email,))
            cur.execute("DELETE FROM users WHERE email=?", (email,))

        self.__con.commit()


@contextmanager
def _managed_cursor(cursor: lite.Cursor):
    try:
        yield cursor
    finally:
        cursor.close()


def _generate_user_id() -> str:
    return str(uuid.uuid1()).replace('-', '')


def _hash_password(password: str) -> str:
    return hashlib.md5(password.encode()).hexdigest()


def _generate_token(email):
    def base64url_encode(input: bytes) -> bytes:
        return base64.urlsafe_b64encode(input).replace(b"=", b"")

    header = '{"alg":"HS256","typ":"JWT"}'
    payload = f'{{"email":"{email}","uuid":"{str(uuid.uuid4())}"}}'
    return base64url_encode(hmac.new("!@#$%^&*RG)))))))JM<==TTTT==>((((((&^HVFT767JJH".encode(), base64url_encode(header.encode()) + b'.' + base64url_encode(payload.encode()), hashlib.sha256).digest()).decode()
