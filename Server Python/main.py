import logging

from fastapi import FastAPI, Request
from fastapi.responses import JSONResponse
from typing import Dict, List
import mysql.connector

app = FastAPI()

# Endpoint per l'autenticazione dei clienti nel database
@app.post("/clienti")
async def post_cliente(request: Request):
    body = await request.json()
    print(body)
    username = body["Username"]
    password = body["Password"]

    conn = mysql.connector.connect(
        host="localhost",
        user="root",
        database="aziendavernici"
    )

    mycursor = conn.cursor()
    sql = "SELECT * FROM Clienti WHERE Username = %s AND Password = %s"
    val = (username, password)
    mycursor.execute(sql, val)
    result = mycursor.fetchone()

    # Close the database connection
    conn.close()

    # Return a boolean indicating whether the credentials exist in the database
    if result:
        return True
    else:
        return False


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

# Endpoint per aggiornare la quantita di un colore
@app.put("/colori")
async def put_colori(request: Request):
    body = await request.json()

    try:
        conn = mysql.connector.connect(
            host="localhost",
            user="root",
            database="aziendavernici"
        )

        # Aggiornamento colore
        cursor = conn.cursor()
        query = "UPDATE Colori SET QuantitaKg = %s WHERE ID = %s"
        values = (body["QuantitaKg"], body["ID"])
        cursor.execute(query, values)
        conn.commit()

        cursor.close()
        conn.close()
        return JSONResponse(content="Quantita' colore correttamente")

    except Exception as e:
        logging.error("Errore durante l'aggiornamento del database", e)

    return JSONResponse(content={"message": "Errore durante l'aggiornamento"})


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

# Endpoint per aggiornare la quantita di un additivo
@app.put("/additivi")
async def put_additivi(request: Request):
    body = await request.json()

    try:
        conn = mysql.connector.connect(
            host="localhost",
            user="root",
            database="aziendavernici"
        )

        # Aggiornamento colore
        cursor = conn.cursor()
        query = "UPDATE Additivi SET QuantitaKg = %s WHERE ID = %s"
        values = (body["QuantitaKg"], body["ID"])
        cursor.execute(query, values)
        conn.commit()

        cursor.close()
        conn.close()
        return JSONResponse(content="Quantita' additivo correttamente")

    except Exception as e:
        logging.error("Errore durante l'aggiornamento del database", e)

    return JSONResponse(content={"message": "Errore durante l'aggiornamento"})


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
            percentuale = colore["Percentuale"]
            query = "INSERT INTO Composizione_Colori (Vernice, Colore, Percentuale) VALUES (%s, %s, %s)"
            values = (idVernice, idColore, percentuale)
            cursor.execute(query, values)
            conn.commit()

        # Inserimento composizione additivi della vernice
        for additivo in body["Formula"]["Additivi"]:
            idAdditivo = additivo["ID"]
            percentuale = additivo["Percentuale"]
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


# Endpoint per effettuare la produzione di una vernice
@app.put("/vernici")
async def update_item(request: Request):
    body = await request.json()
    print(body)

    try:
        conn = mysql.connector.connect(
            host="localhost",
            user="root",
            database="aziendavernici"
        )

        # Aggiornamento vernice
        cursor = conn.cursor()
        query = "UPDATE Vernici SET QuantitaKg = %s WHERE ID = %s"
        values = (body["QuantitaKg"], body["ID"])
        cursor.execute(query, values)
        conn.commit()

        # Inserimento composizione colori della vernice
        for colore in body["Formula"]["Colori"]:
            query = "UPDATE Colori SET QuantitaKg = %s WHERE ID = %s"
            values = (colore["QuantitaKg"], colore["ID"])
            cursor.execute(query, values)
            conn.commit()

        # Inserimento composizione additivi della vernice
        for additivo in body["Formula"]["Additivi"]:
            query = "UPDATE Additivi SET QuantitaKg = %s WHERE ID = %s"
            values = (additivo["QuantitaKg"], additivo["ID"])
            cursor.execute(query, values)
            conn.commit()

        cursor.close()
        conn.close()
        return JSONResponse(content="Produzione ultimata correttamente")

    except Exception as e:
        logging.error("Errore durante l'aggiornamento del database", e)

    return JSONResponse(content={"message": "Errore durante la produzione"})


# Endpoint per l'inserimento degli ordini nel database
@app.post("/ordini")
async def post_ordini(request: Request):
    body = await request.json()
    print(body)

    try:
        conn = mysql.connector.connect(
            host="localhost",
            user="root",
            database="aziendavernici"
        )


        conn.close()
        return JSONResponse(content="Ordine inserito correttamente nel database")
    except Exception as e:
        logging.error("Errore durante l'inserimento nel database", e)

        return JSONResponse(content={"message": "Errore durante l'elaborazione dell'ordine nel database"})
