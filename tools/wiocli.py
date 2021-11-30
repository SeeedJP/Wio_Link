#!/usr/bin/env python3

"""
Usage:
  wiocli database user list [--email <email>]
  wiocli database user add <email> <password> [<max_node_count>]
  wiocli database user remove [--email <email>]
"""

from docopt import docopt
from database import Database


def user_list(args):
    email = args["<email>"]

    db = Database()
    rows = db.get_user_list(email=email)

    for row in rows:
        print("\t".join([row["user_id"], row["email"], row["created_at"], str(row["max_node_count"])]))


def user_add(args):
    email = args["<email>"]
    password = args["<password>"]
    max_node_count = int(args["<max_node_count>"]) if args["<max_node_count>"] is not None else None

    db = Database()
    user_id = db.add_user(email, password, max_node_count)

    print(user_id)


def user_remove(args):
    email = args["<email>"]

    if email is None:
        raise Exception("Please specify email.")

    db = Database()
    db.remove_user(email=email)


def user(args):
    if args["list"]:
        user_list(args)
    elif args["add"]:
        user_add(args)
    elif args["remove"]:
        user_remove(args)


def database(args):
    if args["user"]:
        user(args)


def main(args):
    if args["database"]:
        database(args)


if __name__ == "__main__":
    args = docopt(__doc__)
    main(args)
