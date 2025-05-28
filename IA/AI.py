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

# === CONFIG ===
DEVICE_USB = "/dev/ttyACM0"  # Cambia se necessario
BAUDRATE = 9600
DIM_FRAME = 200

# === INIZIALIZZA SERIAL ===
try:
    ser = serial.Serial(DEVICE_USB, BAUDRATE, timeout=1)
    time.sleep(2)
    print("[✓] Porta seriale aperta.")
except Exception as e:
    print(f"[!] Errore apertura seriale: {e}")
    ser = None

# === FUNZIONE: scatta foto ===
def scatta_foto():
    cap = cv2.VideoCapture(0)
    time.sleep(2)  # attesa attivazione
    for _ in range(5):
        cap.read()  # scarta primi frame
    ret, frame = cap.read()
    cap.release()
    if not ret:
        raise Exception("Errore nella cattura dell'immagine")
    return frame

# === FUNZIONE: estrai HSV medio ===
def estrai_colore_dominante(img):
    resized = cv2.resize(img, (DIM_FRAME, DIM_FRAME))
    hsv = cv2.cvtColor(resized, cv2.COLOR_BGR2HSV)
    media = np.mean(hsv.reshape(-1, 3), axis=0)
    return tuple(media.astype(int))  # (H, S, V)

# === FUNZIONE: decidi valore da inviare in base al colore ===
def decidi_valore(hsv):
    h = hsv[0]
    if (h < 10 or h > 160):   # ROSSO (Hue 0–10 e 160–180)
        print("Colore rilevato: ROSSO")
        return 1
    elif 40 <= h <= 85:       # VERDE (Hue 40–85)
        print("Colore rilevato: VERDE")
        return 2
    elif 100 <= h <= 130:     # BLU (Hue 100–130)
        print("Colore rilevato: BLU")
        return 3
    else:
        print("Colore non riconosciuto.")
        return 0  # Nessun colore valido

# === FUNZIONE: invia comando ===
def invia_comando(valore):
    comando = f"{valore}\n"
    if ser:
        ser.write(comando.encode())
        print(f"[→] Inviato ad Arduino: {valore}")
    else:
        print("[!] Porta seriale non disponibile.")

# === MAIN ===
def main():
<<<<<<< HEAD
    print("Acquisizione immagine...")
    frame = scatta_foto()

    print("Elaborazione colore...")
=======
    print("[📸] Acquisizione immagine...")
    frame = scatta_foto()

    print("[🎨] Elaborazione colore...")
>>>>>>> 42cc626 (bho)
    hsv = estrai_colore_dominante(frame)
    print(f"[HSV] Media: {hsv}")

    valore = decidi_valore(hsv)
<<<<<<< HEAD
    print("Valore passato: "valore)
=======
>>>>>>> 42cc626 (bho)
    invia_comando(valore)

    print(" Operazione completata.")

if __name__ == "__main__":
    main()