#===== FLUSSO LOGICO =====
# 1. Oggetto viene raccolto dal braccio
# 2. Raspberry scatta foto
# 3. Riconosce colore (es. "verde")
# 4. Decide: "verde → quadrato B"
# 5. Invia comando all’ESP32: "muovi verso quadrato B"
# 6. Il braccio esegue il posizionamento


import cv2
import numpy as np
import serial
import time
import json

# ===== PARAMETRI =====
DEVICE_USB = "/dev/ttyUSB0"   
BAUDRATE = 9600
DIMENSIONE_FRAME = 200
DATASET_FILE = "/mnt/data/color_dataset.json"  # Percorso del file JSON -> da modificare quando lo carico su RASP


# ===== INIZIALIZZA COMUNICAZIONE SERIALE =====
try:
    ser = serial.Serial(DEVICE_USB, BAUDRATE, timeout=1)
    time.sleep(2)  # attesa per stabilire connessione
    print("[✓] Connessione seriale stabilita.")
except Exception as e:
    print(f"[!] Errore nella connessione seriale: {e}")
    ser = None

# ===== CARICA COLORI DAL JSON =====
try:
    with open(DATASET_FILE, "r") as f:
        COLORI = json.load(f)
    print("Dataset colori caricato.")
except Exception as e:
    print(f"Errore nel caricamento del dataset: {e}")
    COLORI = {}

# ===== SCATTA FOTO ======
#scatta una foto con la fotocamera e ritorna il frame di essa
def scatta_foto():
    cap = cv2.VideoCapture(0)
    time.sleep(1)  # attesa per il focus
    ret, frame = cap.read()
    cap.release()
    if not ret:
        raise Exception("Errore nella cattura dell'immagine")
    return frame


# ===== ESTRAI COLORE =====
#dato il frame estra il colore dell'oggetto
def estrai_colore_dominante(img):
    resized = cv2.resize(img, (DIMENSIONE_FRAME, DIMENSIONE_FRAME))
    hsv = cv2.cvtColor(resized, cv2.COLOR_BGR2HSV)
    media = np.mean(hsv.reshape(-1, 3), axis=0)
    return tuple(media.astype(int))  # formato del valor e(H, S, V)

# ===== DECIDI COLORE =====
#funzione che esamina il colore estratto e lo confronta con il dataset caricato
def decidi_colore(hsv):
    h = hsv[0]
    for nome_colore, valori in COLORI.items():
        if h >= valori[0] - 10 and h <= valori[0] + 10:  # Confronto con una tolleranza
            print(f"Colore rilevato: {nome_colore}")
            return COLORI[nome_colore][2]  # Restituisce il valore associato al colore
    print("Nessun colore corrispondente trovato.")
    return 0  # Valore di default se non trovato

# ===== INVIA COMANDO =====
#Funzione che invia ad Arduino il valore del movimento da far effettuare al braccio
def invia_comando(valore):
    comando = f"{valore}\n"
    if ser:
        ser.write(comando.encode())
        print(f"Comando inviato: {comando.strip()}")
    else:
        print("Serial non disponibile")

# === MAIN ===
def main():
    print("Scatto della foto in corso...")
    frame = scatta_foto()

    print("Estrazione colore...")
    colore = estrai_colore_dominante(frame)
    print(f"Colore HSV medio: {colore}")

    valore = decidi_colore(colore)
    print(f"Valore da inviare: {valore}")

    invia_comando(valore)

    print("[✅] Fine processo.")

if __name__ == "__main__":
    main()
