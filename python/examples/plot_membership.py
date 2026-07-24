"""
Example demonstrating membership function visualization.
"""

from pathlib import Path

from fuzzyviz.membership import plot_membership


def main():

    project_root = Path(__file__).resolve().parents[2]

    json_file = (
    project_root
    / "out"
    / "build"
    / "x64-Debug"
    / "examples"
    / "output"
    / "membership.json"
)

    print("Loading {}".format(json_file))

    plot_membership(json_file)


if __name__ == "__main__":
    main()