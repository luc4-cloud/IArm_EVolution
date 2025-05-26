# APPROFONDIMENTO - CREAZIONE DELL’INTELLIGENZA ARTIFICIALE

---

Quello che abbiamo realizzato è un sistema di Intelligenza Artificiale capace di prendere decisioni in modo autonomo, in base all’input ricevuto. Nel nostro caso, l’AI riconosce il colore dell’oggetto raccolto da un braccio robotico e decide dove posizionarlo, associandolo al quadrato del colore corrispondente. Tutto avviene senza intervento umano, grazie all’integrazione tra visione artificiale, logica decisionale e controllo meccanico.

Di seguito sono riportati i passaggi principali che abbiamo seguito per creare questo sistema:

### PASSO 1 - Implementazione dell’ambiente virtuale

Ho scelto di implementare un ambiente virtuale per un motivo molto chiaro:

- **Isolamento delle dipendenze**
    
    Cosi da evitare che si presentino conflitti con librerie usate in progetti diversi e salvare tutto in una dipendenza “*requirements.txt*” per essere salvato su un dispositivo esterno 
    

Per l’implementazione di un ambiente virtuale aprire il terminale e:

1. Creare una cartella 

```jsx
mkdir IArm_EVolution
```

1. Entra nella directory creata

```jsx
cd IArm_EVolution
```

1. Crea l’ambiente virtuale 

```jsx
python3 -m venv venv
```

1. Attiva l’ambiente virtuale 

```jsx
source venv/bin/activate
```

Se i passi sopra saranno andati a buon fine quello che vedrai sarà più o meno questo

```jsx
(venv) pi@raspberrypi:~/IArm_EVolution $
```

### PASSO 2 - Scaricamento delle librerie necessarie

Dopo la creazione dell’ambiente virtuale opero all’interno di esso e vado a scaricare le librerie che devo utilizzare:

- **OpenCV**
- **Numpy**

Per scaricare tutto il necessario aprire il terminale e:

1. OpenCV 

```jsx
pip install opencv-python 
```

1. Numpy

```jsx
pip install numpy
```

### PASSO 3 - Implementazione dataset dei colori

Dopo aver scaricato le librerie è necessario implementare un dataset per i colori riconoscibili dall’IA, la struttura di questo file è la corrente:

```python
"nome_colore": [valore, valore, valore],
```

per implementare il dataset è necessario:

1. Apri il terminale (N.B → è necessario essere nella cartella del progetto) e creare il file .JSON

```python
nano color_dataset.json
```

1. Inserire i colori da far riconoscere 

```python
{
  "rosso": [0, 255, 255],
  "arancione": [15, 255, 255],
  "giallo": [30, 255, 255],
  "verde": [60, 255, 255],
  "turchese": [90, 255, 255],
  "azzurro": [100, 200, 255],
  "blu": [120, 255, 255],
  "indaco": [135, 255, 255],
  "viola": [150, 255, 255],
  "rosa": [170, 150, 255],
  "bianco": [0, 0, 255],
  "grigio": [0, 0, 127],
  "nero": [0, 0, 0],
  "marrone": [20, 255, 100],
  "lime": [75, 255, 255],
  "acqua": [85, 200, 255],
  "fucsia": [160, 255, 255],
  "oro": [25, 255, 200],
  "argento": [0, 0, 200],
  "beige": [20, 50, 220]
}
```

### PASSO 4 - Creazione del codice

dopo aver installato le librerie occorre creare il file python per la gestione dell’IA, per far ciò:

1. Aprire il terminale e recarsi nella cartella del progetto 

```jsx
nano AI.py
```

1. Dopo aver creato il file dobbiamo scriverci dentro
    1. *Inizializzazione porte seriali*
    
    ```python
    try:
        ser = serial.Serial(DEVICE_USB, BAUDRATE, timeout=1)
        time.sleep(2)  # attesa per stabilire connessione
        print("[✓] Connessione seriale stabilita.")
    except Exception as e:
        print(f"[!] Errore nella connessione seriale: {e}")
        ser = None
    ```
    
    b. *Carica colori dal file “color_dataset.json”*  
    
    ```python
    try:
        with open(DATASET_FILE, "r") as f:
            COLORI = json.load(f)
        print("Dataset colori caricato.")
    except Exception as e:
        print(f"Errore nel caricamento del dataset: {e}")
        COLORI = {}
    ```
    

c. *Fare uno scatto dell’oggetto raccolto*

```python
def scatta_foto():
    cap = cv2.VideoCapture(0)
    time.sleep(1)  # attesa per il focus
    ret, frame = cap.read()
    cap.release()
    if not ret:
        raise Exception("Errore nella cattura dell'immagine")
    return frame
```

d. D*alla foto scattata estrae il colore*

```python
def estrai_colore_dominante(img):
    resized = cv2.resize(img, (DIMENSIONE_FRAME, DIMENSIONE_FRAME))
    hsv = cv2.cvtColor(resized, cv2.COLOR_BGR2HSV)
    media = np.mean(hsv.reshape(-1, 3), axis=0)
    return tuple(media.astype(int)) 
```

e. In base al valore estratto effettua il paragone con il dataset per decidere il colore

```python
def decidi_colore(hsv):
    h = hsv[0]
    for nome_colore, valori in COLORI.items():
        if h >= valori[0] - 10 and h <= valori[0] + 10:  # Confronto con una tolleranza
            print(f"Colore rilevato: {nome_colore}")
            return COLORI[nome_colore][2]  # Restituisce il valore associato al colore
    print("Nessun colore corrispondente trovato.")
    return 0  # Valore di default se non trovato
```

f. Dopo la decisione presa invia il comando ad Arduino

```python
def invia_comando(valore):
    comando = f"{valore}\n"
    if ser:
        ser.write(comando.encode())
        print(f"Comando inviato: {comando.strip()}")
    else:
        print("Serial non disponibile")
```

g. Infine l’implementazione del main
```python
def main():
    print("Scatto della foto in corso...")
    frame = scatta_foto()

    print("Estrazione colore...")
    colore = estrai_colore_dominante(frame)
    print(f"Colore HSV medio: {colore}")

    valore = decidi_colore(colore)
    print(f"Valore da inviare: {valore}")

    invia_comando(valore)

    print("Fine processo.")

if __name__ == "__main__":
    main()
```
