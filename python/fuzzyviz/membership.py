"""
Visualization of sampled membership functions exported by JsonExporter.
"""

from pathlib import Path
import matplotlib.pyplot as plt
from .common import load_visualization
def plot_membership(
    json_file,
    save=None,
    show=True
):

    data = load_visualization(
        json_file,
        expected_type="membership"
    )

    variable = data["variable"]
    sets = data["sets"]

    plt.figure(figsize=(8, 5))

    for fuzzy_set in sets:

        points = fuzzy_set["points"]

        x = [point["x"] for point in points]
        y = [point["membership"] for point in points]

        plt.plot(
            x,
            y,
            linewidth=2,
            label="{}".format(fuzzy_set["name"])
        )

    plt.title(
        "Membership Functions - {}".format(
            variable["name"]
        )
    )

    plt.xlabel(variable["name"])
    plt.ylabel("Membership")

    plt.xlim(
        variable["min"],
        variable["max"]
    )

    plt.ylim(-0.05, 1.05)

    plt.grid(True)
    plt.legend()

    plt.tight_layout()

    if save is not None:
        save = Path(save)
        save.parent.mkdir(
            parents=True,
            exist_ok=True
        )
        plt.savefig(save)

    if show:
        plt.show()

    plt.close()