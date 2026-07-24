#visualisation utilities for clustering

from pathlib import Path
import matplotlib.pyplot as plt
import numpy as np

from .common import load_visualization


def plot_clusters(json_file, save=None, show=True):
  

    data = load_visualization(json_file, "fcm")
    dataset = data["dataset"]
    points = np.asarray([point["coordinates"] for point in dataset])
    centers = np.asarray(data["centers"])
    memberships = np.asarray([point["memberships"] for point in dataset])

    if points.shape[1] != 2:
        raise ValueError(
            "Cluster visualization currently supports only 2-D datasets."
        )

    cluster_ids = np.argmax(memberships, axis=1)

    plt.figure(figsize=(8, 6))

    plt.scatter(
        points[:, 0],
        points[:, 1],
        c=cluster_ids,
        cmap="viridis",
        s=40,
        alpha=0.75,
        edgecolors="black",
        linewidths=0.3,
        label="Data Points",
    )

    plt.scatter(
        centers[:, 0],
        centers[:, 1],
        marker="X",
        s=220,
        c="red",
        edgecolors="black",
        linewidths=1.0,
        label="Cluster Centers",
    )
    query = np.asarray(
    data["queryPoint"]["coordinates"]
)

    plt.scatter(
        query[0],
        query[1],
        marker="*",
        s=250,
        c="orange",
        edgecolors="black",
        linewidths=1.0,
        label="Query Point"
)

    for index, center in enumerate(centers):
        plt.text(
            center[0],
            center[1],
            "C{}".format(index),
            fontsize=10,
            weight="bold",
            ha="left",
            va="bottom",
        )
    query = data["queryPoint"]

    membership_text = (
    "Query Point\n\n"
    "Predicted Cluster: C{}\n\n"
    ).format(query["predictedCluster"])

    membership_text += "Memberships\n"

    for index, value in enumerate(query["memberships"]):
        membership_text += "C{} : {:.3f}\n".format(
            index,
            value
        )

    plt.gcf().text(0.80,
    0.75,
    membership_text,
    fontsize=10,
    verticalalignment="top",
    bbox=dict(
        facecolor="white",
        edgecolor="black",
        boxstyle="round"
    )
)
    plt.title("Fuzzy C-Means Clustering")
    feature_names = data["featureNames"]
    plt.xlabel(feature_names[0])
    plt.ylabel(feature_names[1])

    plt.grid(True)
    plt.legend()

    plt.tight_layout()

    if save:
        plt.savefig(Path(save), dpi=300)

    if show:
        plt.show()

    plt.close()
