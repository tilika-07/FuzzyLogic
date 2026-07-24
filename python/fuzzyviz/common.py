import json
from pathlib import Path
def load_json(json_file):
  

    path = Path(json_file)

    if not path.exists():
        raise FileNotFoundError(f"File not found: {path}")

    if not path.is_file():
        raise ValueError(f"'{path}' is not a file.")

    with path.open("r", encoding="utf-8") as f:
        return json.load(f)


def validate_type(data, expected_type):
   

    actual = data.get("type")

    if actual != expected_type:
        raise ValueError(
            f"Expected visualization type '{expected_type}', "
            f"but found '{actual}'."
        )


def require_keys(data, keys):
 

    missing = [key for key in keys if key not in data]

    if missing:
        raise KeyError(
            f"Missing required key(s): {', '.join(missing)}"
        )


def load_visualization(json_file, expected_type):

    data = load_json(json_file)

    validate_type(data, expected_type)

    if expected_type == "membership":
        require_keys(
            data,
            ["type", "variable", "sets"]
        )

    elif expected_type == "fcm":
        require_keys(
            data,
            [
                "type",
                "featureNames",
                "dataset",
                "centers",
                "queryPoint"
            ]
        )

    return data