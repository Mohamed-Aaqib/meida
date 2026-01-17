import networkx as nx
from pathlib import Path

class GraphBuilder:

    def __init__(self):
        self.graph = nx.MultiDiGraph()
        # self.output_path

    def add_redlation(self,e1, rel ,e2, chunk_id):
        self.graph.add_node(e1)
        self.graph.add_node(e2)
        self.graph.add_edge(e1,e2,relational=rel,soruce=chunk_id)

    def save(self,path):
        Path(path).parent.mkdir(parents=True, exist_ok=True)
        nx.write_gpickle(self.graph,path)


