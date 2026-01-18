from groq import Groq
import os


client = Groq(
    api_key="testkey"
)

def call_groq(prompt:str):

    res = client.chat.completions.create(
        model = "llama3-8b-8192",
        messages= [
            {"role":"system","content":"You are a precise technical assistant."},
            {"role": "user", "content": prompt}
        ],
        temperature=0.2,
        max_tokens=512
    )

    return res.choices[0].message.content

