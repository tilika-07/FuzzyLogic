"""
Example demonstrating Fuzzy C-Means cluster visualization.
"""

from pathlib import Path

from fuzzyviz.clustering import plot_clusters


def main():

    project_root = Path(__file__).resolve().parents[2]

    json_file = (
        project_root
        / "out"
        / "build"
        / "x64-Debug"
        / "examples"
        / "output"
        / "clusters.json"
    )

    print("Loading {}".format(json_file))

    plot_clusters(json_file)


if __name__ == "__main__":
    main()