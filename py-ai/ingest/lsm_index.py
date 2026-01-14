import orjson
from pathlib import Path

class LSMIndex:

    def __init__(self,base="storage/lsm"):
        self.base = Path(base)
        self.base.mkdir(parents=True, exist_ok=True)
        self.segment_id = len(list(self.base.glob("segment_*.json")))

    def write_segment(self,records:list[dict]):
        path = self.base / f"segment_{self.segment_id:03d}.json"
        with open(path,"wb") as f:
            f.write(orjson.dumps(records))
        self.segment_id += 1