from multiprocessing import Pool, cpu_count
from tqdm import tqdm

from ingest.entity_extractor import extract_entities
from ingest.fingerprint import fingerprint
from ingest.graph_builder import GraphBuilder
from ingest.load_chunks import load_chunks
from ingest.lsm_index import LSMIndex
from ingest.relation_extractor import extract_relation_optimized


def proccess_batch(batch):

    texts = [ c["text"] for c in batch]
    entities_batch = extract_entities(texts)
    out = []

    for c, ents in zip(batch,entities_batch):
        rels = extract_relation_optimized(c["text"],ents)
        out.append(rels)

    return out




def run_indexing(input_fldr="../output"):

    chunks = load_chunks(input_fldr)
    lsm = LSMIndex()
    graph = GraphBuilder()

    batch_size = 100
    segment = []

    batches = [
        chunks[i:i+batch_size]
        for i in range(0,len(chunks),batch_size)
    ]

    # input until now is ambiguous for process_batch the fields, text:{}
    with Pool(cpu_count()) as pool:
        for results in tqdm(pool.imap(proccess_batch,batches),total=len(batches)):
            for c, relations in results:
                for e1, rel ,e2 in relations:
                    segment.append({
                        "fingerprint":fingerprint(e1,rel,e2,c["text"]),
                        "e1":e1,
                        "rel":rel,
                        "e2": e2,
                        "chunk_id":c["chunk_id"]
                    })
                    graph.add_redlation(e1,rel,e2,c["chunk_id"])

                if(len(segment) >= 5000):
                    lsm.write_segment(segment)
                    segment.clear()
    if segment:
        lsm.write_segment(segment)

    # proper output path required
    graph.save(input_fldr)

if __name__ == "__main__":
    run_indexing()
