from fastapi import FastAPI, Request
from fastapi.responses import JSONResponse
from typing import Dict, List
import mysql.connector

app = FastAPI()

# Endpoint per ottenere i colori dal database
@app.get("/colori")
def get_colori():
    # Connessione al database
    conn = mysql.connector.connect(
        host="localhost",
        user="root",
        database="aziendavernici"
    )

    # Creazione del cursore per effettuare le query
    cursor = conn.cursor()

    # Query di selezione dei campi Nome e QuantitaKg dalla tabella Colori
    query = "SELECT * FROM Colori"

    # Esecuzione della query
    cursor.execute(query)

    # Recupero dei risultati
    results = cursor.fetchall()

    # Chiusura della connessione e del cursore
    cursor.close()
    conn.close()

    # Creazione della lista di colori da restituire come JSON
    colori = []
    for result in results:
        ID = result[0]
        nome = result[1]
        quantitaKg = result[2]
        colore = {"ID": ID, "nome": nome, "quantitaKg": quantitaKg}
        colori.append(colore)

    # Restituzione della lista di colori come JSON
    return {"colori": colori}

# Endpoint per l'inserimento dei colori nel database
@app.post("/colori")
async def post_colori(request: Request):
    body = await request.json()
    nome = body["Nome"]
    quantitaKg = body["QuantitaKg"]

    try:
        conn = mysql.connector.connect(
            host="localhost",
            user="root",
            database="aziendavernici"
        )
        cursor = conn.cursor()
        query = "INSERT INTO Colori (Nome, QuantitaKg) VALUES (%s, %s)"
        values = (nome, quantitaKg)
        cursor.execute(query, values)
        conn.commit()
        cursor.close()
        conn.close()
        return JSONResponse(content="Colore inserito correttamente nel database")
    except Exception as e:
        print("Errore durante l'inserimento del colore nel database:", e)
        return JSONResponse(content={"message": "Errore durante l'inserimento del colore nel database"})

# Endpoint per ottenere gli additivi dal database
@app.get("/additivi")
def get_additivi():
    # Connessione al database
    conn = mysql.connector.connect(
        host="localhost",
        user="root",
        database="aziendavernici"
    )

    # Creazione del cursore per effettuare le query
    cursor = conn.cursor()

    # Query di selezione dei campi Nome e QuantitaKg dalla tabella Colori
    query = "SELECT * FROM Additivi"

    # Esecuzione della query
    cursor.execute(query)

    # Recupero dei risultati
    results = cursor.fetchall()

    # Chiusura della connessione e del cursore
    cursor.close()
    conn.close()

    # Creazione della lista di colori da restituire come JSON
    additivi = []
    for result in results:
        ID = result[0]
        nome = result[1]
        quantitaKg = result[2]
        additivo = {"ID": ID, "nome": nome, "quantitaKg": quantitaKg}
        additivi.append(additivo)

    # Restituzione della lista di colori come JSON
    return {"additivi": additivi}

# Endpoint per l'inserimento degli additivi nel database
@app.post("/additivi")
async def post_additivi(request: Request):
    body = await request.json()
    nome = body["Nome"]
    quantitaKg = body["QuantitaKg"]

    try:
        conn = mysql.connector.connect(
            host="localhost",
            user="root",
            database="aziendavernici"
        )
        cursor = conn.cursor()
        query = "INSERT INTO Additivi (Nome, QuantitaKg) VALUES (%s, %s)"
        values = (nome, quantitaKg)
        cursor.execute(query, values)
        conn.commit()
        cursor.close()
        conn.close()
        return JSONResponse(content="Additivo inserito correttamente nel database")
    except Exception as e:
        print("Errore durante l'inserimento dell'additivo nel database:", e)
        return JSONResponse(content={"message": "Errore durante l'inserimento dell'additivo nel database"})
