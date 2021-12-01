import PySimpleGUI as sg

layout = [[sg.Text("Hello world")]]

window = sg.Window("EVE", layout)

while True:
    event, values = window.read()

    if event == sg.WINDOW_CLOSED or event == 'Quit':
        break

window.close()
