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
- **pyserial**

Per scaricare tutto il necessario aprire il terminale e:

1. OpenCV 

```jsx
pip install opencv-python 
```

1. Numpy

```jsx
pip install numpy
```

1. pyserial

```jsx
pip install pyserial
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

1. Inserire i colori da far riconoscere - utilizza il file "color_dataset.json" nella cartella IA

### PASSO 4 - Creazione del codice

dopo aver installato le librerie occorre creare il file python per la gestione dell’IA, per far ciò:

1. Aprire il terminale e recarsi nella cartella del progetto 

```jsx
nano AI.py
```

Successivamente copiare il contenuto del file "AI.py" dalla repository e incollare nel file appena creato

### PASSO 5 - Collegare la WEBCAM a Raspberry

Per il corretto funzionamento è necessario che venga collegata ad una porta usb di Raspberry una videocamera (Non è necessario la camera proprietaria).

N.B. Per verificare che la fotocamera sia correttamente collegata: aprire il terminale e scrivere il seguente comando
```jsx
lsusb
```

Se collegata correttamente si vedrà il modello della webcam collegata nella porta USB inserita
