from bottle import route
from bottle import run
from bottle import template
from bottle import static_file

@route('/hello/<name>')
def index(name):
    return template('<b>Hello {{name}}</b>!', name=name)

@route('/static/<filename>')
def canvas(filename):
    return static_file(filename, root='./static/')

run(host='localhost', port=8080)
