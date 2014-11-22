import subprocess

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
    shader = subprocess.check_output(["./ppix", "-m", "-r"])
    return shader

def get_shaderp_str():
    shader = subprocess.check_output(["python", "randomart.py"])
    return shader

@route('/shade')
def shade():
    shaderstr = get_shader_str()
    return template('./templates/copytimetest.html', shaderstr=shaderstr)

@route('/shadep')
def shadep():
    shaderstr = get_shaderp_str()
    return template('./templates/copytimetest.html', shaderstr=shaderstr)

run(host='localhost', port=8080)
