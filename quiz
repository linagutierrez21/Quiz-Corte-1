import cv2
import mediapipe as mp
import serial
import time

# Puerto de tu Arduino
SERIAL_PORT = 'COM6' 
BAUD_RATE = 9600

# Inicializar Comunicación Serial
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2) # Estabilización
    print(f"--- Conectado a Arduino en {SERIAL_PORT} ---")
except Exception as e:
    print(f"Error Serial: {e}. Continuando sin Arduino...")
    ser = None

# Inicializar MediaPipe Pose
mp_pose = mp.solutions.pose
mp_drawing = mp.solutions.drawing_utils
pose = mp_pose.Pose(min_detection_confidence=0.5, min_tracking_confidence=0.5)

# Captura de Video
cap = cv2.VideoCapture(0)

print("Presiona 'q' para salir.")

last_state = None

while cap.isOpened():
    success, image = cap.read()
    if not success:
        print("Error al acceder a la cámara.")
        break

    # Procesar imagen
    image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    results = pose.process(image_rgb)

    status_text = "Buscando..."
    if results.pose_landmarks:
        # Dibujar landmarks
        mp_drawing.draw_landmarks(image, results.pose_landmarks, mp_pose.POSE_CONNECTIONS)

        # Extraer puntos clave (Cadera y Rodilla derecha)
        landmarks = results.pose_landmarks.landmark
        hip_y = landmarks[mp_pose.PoseLandmark.RIGHT_HIP].y
        knee_y = landmarks[mp_pose.PoseLandmark.RIGHT_KNEE].y

        # Lógica de clasificación: Distancia vertical normalizada
        diff = abs(knee_y - hip_y)
        
        # Umbral ajustable según la distancia a la cámara
        if diff > 0.15:
            status_text = "PARADO"
            current_state = 'S'
            color = (0, 0, 255) # Rojo (BGR)
        else:
            status_text = "SENTADO"
            current_state = 'C'
            color = (255, 0, 0 ) # azul (BGR)

        # Enviar a Arduino si el estado cambia
        if ser and current_state != last_state:
            ser.write(current_state.encode())
            last_state = current_state

    # Visualización de la etiqueta
    cv2.putText(image, f"Estado: {status_text}", (50, 50), 
                cv2.FONT_HERSHEY_SIMPLEX, 1, color if 'color' in locals() else (255, 255, 255), 2)

    cv2.imshow('MediaPipe Pose - Telecom Eng Tool', image)

    if cv2.waitKey(5) & 0xFF == ord('q'):
        break

# Limpieza
cap.release()
cv2.destroyAllWindows()
if ser:
    ser.close()
pose.close()
