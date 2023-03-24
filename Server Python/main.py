import logging

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

# Endpoint per l'inserimento delle vernici nel database
@app.post("/vernici")
async def post_vernici(request: Request):
    body = await request.json()
    nome = body["Nome"]
    prezzoKg = body["PrezzoKg"]

    try:
        conn = mysql.connector.connect(
            host="localhost",
            user="root",
            database="aziendavernici"
        )
        # Inserimento vernice
        cursor = conn.cursor()
        query = "INSERT INTO Vernici (Nome, PrezzoKg) VALUES (%s, %s)"
        values = (nome, prezzoKg)
        cursor.execute(query, values)
        conn.commit()
        idVernice = cursor.lastrowid

        # Inserimento composizione colori della vernice
        for colore in body["Formula"]["Colori"]:
            idColore = colore["ID"]
            percentuale = colore["percentuale"]
            query = "INSERT INTO Composizione_Colori (Vernice, Colore, Percentuale) VALUES (%s, %s, %s)"
            values = (idVernice, idColore, percentuale)
            cursor.execute(query, values)
            conn.commit()

        # Inserimento composizione additivi della vernice
        for additivo in body["Formula"]["Additivi"]:
            idAdditivo = additivo["ID"]
            percentuale = additivo["percentuale"]
            query = "INSERT INTO Composizione_Additivi (Vernice, Additivo, Percentuale) VALUES (%s, %s, %s)"
            values = (idVernice, idAdditivo, percentuale)
            cursor.execute(query, values)
            conn.commit()

        cursor.close()
        conn.close()
        return JSONResponse(content="Vernice inserita correttamente nel database")
    except Exception as e:
        logging.error("Errore durante l'inserimento nel database", e)

        return JSONResponse(content={"message": "Errore durante l'inserimento della vernice nel database"})

    # Endpoint per ottenere gli additivi dal database

@app.get("/vernici")
def get_vernici():
    # Connessione al database
    conn = mysql.connector.connect(
        host="localhost",
        user="root",
        database="aziendavernici"
    )

    # Creazione del cursore per effettuare le query
    cursor = conn.cursor()

    # Query di selezione dalla tabella Vernici
    query = "SELECT * FROM Vernici"

    # Esecuzione della query
    cursor.execute(query)

    # Recupero dei risultati
    results = cursor.fetchall()

    # Creazione della lista di vernici da restituire come JSON
    vernici = []
    for result in results:
        id = result[0]
        nome = result[1]
        quantitaKg = result[2]
        prezzoKg = result[3]
        vernice = {"ID": id, "nome": nome, "quantitaKg": quantitaKg, "PrezzoKg": prezzoKg}
        vernici.append(vernice)

    for vernice in vernici:
        colori = []
        # Query di selezione di tutte i colori che compongono la vernice
        query = "SELECT composizione_colori.Colore, composizione_colori.Percentuale FROM Vernici INNER JOIN composizione_colori ON vernici.id = composizione_colori.vernice WHERE vernici.id = %s"
        cursor.execute(query, (vernice["ID"],))
        results = cursor.fetchall()
        for result in results:
            idColore = result[0]
            percentuale = result[1]
            colore = {"IDColore": idColore, "Percentuale": percentuale}
            colori.append(colore)
        vernice["colori"] = colori

    for vernice in vernici:
        additivi = []
        # Query di selezione di tutte i colori che compongono la vernice
        query = "SELECT composizione_additivi.Additivo, composizione_additivi.Percentuale FROM Vernici INNER JOIN composizione_additivi ON vernici.id = composizione_additivi.vernice WHERE vernici.id = %s"
        cursor.execute(query, (vernice["ID"],))
        results = cursor.fetchall()
        for result in results:
            idAdditivo = result[0]
            percentuale = result[1]
            additivo = {"IDAdditivo": idAdditivo, "Percentuale": percentuale}
            additivi.append(additivo)
        vernice["additivi"] = additivi

    # Chiusura della connessione e del cursore
    cursor.close()
    conn.close()

    # Restituzione della lista di colori come JSON
    return {"vernici": vernici}

# Endpoint per la richiesta PUT
@app.put("/vernici")
async def update_item(request: Request):
    body = await request.json()
    print(body)

    # Connessione al database
    conn = mysql.connector.connect(
        host="localhost",
        user="root",
        database="aziendavernici"
    )

    # Aggiornamento delle quantitaKg dei componenti
    for componente in body["Componenti"]:
        print("ID = ", componente["ID"], ", Nuova q.ta Kg = ", componente["QuantitaKg"])
        query = "U"
        #values = (idVernice, idColore, percentuale)
        #cursor.execute(query, values)
        #conn.commit()

    vernice = body["Vernice"]
    print("ID Vernice = ", vernice["ID"], ", nuova q.ta Kg = ", vernice["QuantitaKg"])

    # cursor = conn.cursor()
    #sql = "UPDATE items SET QuantitaKg = QuantitaKg - %s WHERE id = %s"
    #val = (item_update.quantity, item_id)
    #cursor.execute(sql, val)

    #if cursor.rowcount == 0:
        #raise HTTPException(status_code=404, detail="Item not found")
    return {"ok"}