import randomart

from bottle import route
from bottle import run
from bottle import template
from bottle import static_file

@route('/static/<filename>')
def static(filename):
    return static_file(filename, root='./static/')

def get_shaderp_str():
    art = randomart.Art()
    return art.MakeArt()

def get_shaders_str(seed):
    art = randomart.Art(size=1, seed=seed)
    return art.MakeArt()

@route('/shadep')
def shadep():
    shaderstr = get_shaderp_str()
    return template('./templates/copytimetest.html', shaderstr=shaderstr)

@route('/shades/<seed>')
def shades(seed):
    shaderstr = get_shaders_str(seed)
    return template('./templates/copytimetest.html', shaderstr=shaderstr)

@route('/')
def home():
    return static_file("index.html", root="./static/")

run(host='localhost', port=8080)
