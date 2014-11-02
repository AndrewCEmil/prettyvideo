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

def get_shader_str():
    return 'gl_FragColor = vec4(abs(sin(vTime.x * 3.14159)), 0.0, 0.0, 1.0);'

@route('/shade')
def shade():
    shaderstr = get_shader_str()
    return template('./templates/copytimetest.html', shaderstr=shaderstr)

run(host='localhost', port=8080)
