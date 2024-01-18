import keyboard
import pyautogui
import os

def record_coordinates():
    coordinates = []
    is_recording = True
    file_path = 'recorded_coordinates.cfg'

    print("Нажмите клавишу 'p' для начала записи и для отметки координат, нажмите 'Enter' для сохранения и завершения записи.")

    while True:
        try:
            event = keyboard.read_event(suppress=True)

            if event.event_type == keyboard.KEY_DOWN:

                if is_recording and (event.name == 'p' or event.name == 'р'):
                    # Получаем текущие координаты мыши и записываем их
                    x, y = pyautogui.position()
                    coordinates.append((x, y))
                    print(f"Записаны координаты: {x}, {y}")

                elif is_recording and event.name == 'enter':
                    # Сохраняем координаты в файл и завершаем запись
                    with open(file_path, 'w') as file:
                        for coord in coordinates:
                            file.write(f"{coord[0]}, {coord[1]}\n")
                        print("Координаты сохранены в файл.")
                    is_recording = False
                    coordinates = []  # Очищаем список координат
                    print("Запись завершена.")
                    break

        except KeyboardInterrupt:
            break

record_coordinates()
