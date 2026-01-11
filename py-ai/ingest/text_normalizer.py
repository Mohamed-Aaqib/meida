import re

def normalize(text:str):
    text = text.lower()
    text = re.sub(r"\s+"," ",text)
    # regex, ^ means not, so a-z, 0-9 or white or dashes, may want to include brackets as well
    text = re.sub(r"[^a-z0-9\s\-'\.,\(\)\[\]]"," ",text)

    return text.strip()
