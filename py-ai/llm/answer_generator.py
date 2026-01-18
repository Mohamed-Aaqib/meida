from llm.groq_client import call_groq
from llm.prompt_builder import build_prompt


def generate_answer(context:str):

    prompt = build_prompt(context)
    return call_groq(prompt)
