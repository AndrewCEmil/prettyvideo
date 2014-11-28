#!/usr/bin/python

# Copyright (c) 2010, Andrej Bauer, http://andrej.com/
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#     * Redistributions of source code must retain the above copyright notice,
#       this list of conditions and the following disclaimer.
#
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

######################################################################
# SIMPLE RANDOM ART IN PYTHON
#
# Version 2010-04-21
#
# I get asked every so often to release the source code for my random art
# project at http://www.random-art.org/. The original source is written in Ocaml
# and is not publicly available, but here is a simple example of how you can get
# random art going in python in 250 lines of code.
#
# The idea is to generate expression trees that describe an image. For each
# point (x,y) of the image we evaluate the expression and get a color. A color
# is represented as a triple (r,g,b) where the red, green, blue components are
# numbers between -1 and 1. In computer graphics it is more usual to use the
# range [0,1], but since many operations are symmetric with respect to the
# origin it is more convenient to use the interval [-1,1].
#
# I kept the program as simple as possible, and independent of any non-standard
# Python libraries. Consequently, a number of improvements and further
# experiments are possible:
#
#   * The most pressing problem right now is that the image is displayed as a
#     large number of rectangles of size 1x1 on the tkinter Canvas, which
#     consumes a great deal of memory. You will not be able to draw large images
#     this way. An improved version would use the Python imagining library (PIL)
#     instead.
#
#   * The program uses a simple RGB (Red Green Blue) color model. We could also
#     use the HSV model (Hue Saturation Value), and others. One possibility is
#     to generate a palette of colors and use only colors that are combinations
#     of those from the palette.
#
#   * Of course, you can experiment by introducing new operators. If you are going
#     to play with the source, your first exercise should be a new operator.
#
#   * The program uses cartesian coordinates. You could experiment with polar
#     coordinates.
#
# For more information and further discussion, see http://math.andrej.com/category/random-art/

import math
import random

# Utility functions

curId = 0
def getNextID():
    global curId
    curId += 1
    return curId - 1

def average(c1, c2, w=0.5):
    '''Compute the weighted average of two colors. With w = 0.5 we get the average.'''
    (r1,g1,b1) = c1
    (r2,g2,b2) = c2
    r3 = w * r1 + (1 - w) * r2
    g3 = w * g1 + (1 - w) * g2
    b3 = w * b1 + (1 - w) * b2
    return (r3, g3, b3)

def rgb(r,g,b):
    '''Convert a color represented by (r,g,b) to a string understood by tkinter.'''
    u = max(0, min(255, int(128 * (r + 1))))
    v = max(0, min(255, int(128 * (g + 1))))
    w = max(0, min(255, int(128 * (b + 1))))
    return '#%02x%02x%02x' % (u, v, w)

def well(x):
    '''A function which looks a bit like a well.'''
    return 1 - 2 / (1 + x*x) ** 8

def tent(x):
    '''A function that looks a bit like a tent.'''
    return 1 - 2 * abs(x)

# We next define classes that represent expression trees.

# Each object that reprents and expression should have an eval(self,x,y,t) method
# which computes the value of the expression at (x,y). The __init__ should
# accept the objects representing its subexpressions. The class definition
# should contain the arity attribute which tells how many subexpressions should
# be passed to the __init__ constructor.

class VariableX():
    arity = 0
    def __init__(self):
        self.id = getNextID()
    def __repr__(self): return "x"
    def printme(self, depth):
        print ("  " * depth) + "X"
    def eval(self,x,y,t): return (x,x,x)
    def getShader(self):
        return "vec3 val%d = vec3(vPosition.x);\n" % self.id

class VariableY():
    arity = 0
    def __init__(self):
        self.id = getNextID()
    def __repr__(self): return "y"
    def printme(self, depth):
        print ("  " * depth) + "Y"
    def eval(self,x,y,t): return (y,y,y)
    def getShader(self):
        return "vec3 val%d = vec3(vPosition.y);\n" % self.id

class VariableT():
    arity = 0
    def __init__(self):
        self.id = getNextID()
    def __repr__(self): return "t"
    def printme(self, depth):
        print ("  " * depth) + "Constant"
    def eval(self,x,y,t): return (t,t,t)
    def getShader(self):
        return "vec3 val%d = vec3(vTime.x);\n" % self.id

class Constant():
    arity = 0
    def __init__(self):
        self.id = getNextID()
        self.c = (random.uniform(0,1), random.uniform(0,1), random.uniform(0,1))
    def __repr__(self):
        return 'Constant(%g,%g,%g)' % self.c
    def printme(self, depth):
        print ("  " * depth) + "Constant"
    def eval(self,x,y,t): return self.c
    def getShader(self):
        return "vec3 val{0} = vec3({1}, {2}, {3});\n".format(self.id, self.c[0], self.c[1], self.c[2])


class Sum():
    arity = 2
    def __init__(self, e1, e2):
        self.id = getNextID()
        self.e1 = e1
        self.e2 = e2
    def __repr__(self):
        return 'Sum(%s, %s)' % (self.e1, self.e2)
    def printme(self, depth):
        print ("  " * depth) + "Sum"
        self.e1.printme(depth + 1)
        self.e2.printme(depth + 1)
    def eval(self,x,y,t):
        return average(self.e1.eval(x,y,t), self.e2.eval(x,y,t))
    def getShader(self):
        substr = self.e1.getShader()
        substr += self.e2.getShader()
        substr += "vec3 val{0} = (val{1} + val{2}) / 2.0;\n".format(self.id, self.e1.id, self.e2.id)
        return substr

class Product():
    arity = 2
    def __init__(self, e1, e2):
        self.id = getNextID()
        self.e1 = e1
        self.e2 = e2
    def __repr__(self):
        return 'Product(%s, %s)' % (self.e1, self.e2)
    def printme(self, depth):
        print ("  " * depth) + "Product"
        self.e1.printme(depth + 1)
        self.e2.printme(depth + 1)
    def eval(self,x,y,t):
        (r1,g1,b1) = self.e1.eval(x,y,t)
        (r2,g2,b2) = self.e2.eval(x,y,t)
        r3 = r1 * r2
        g3 = g1 * g2
        b3 = b1 * b2
        return (r3, g3, b3)
    def getShader(self):
        substr = self.e1.getShader()
        substr += self.e2.getShader()
        substr += "vec3 val{0} = val{1} * val{2};\n".format(self.id, self.e1.id, self.e2.id)
        return substr

class Mod():
    arity = 2
    def __init__(self, e1, e2):
        self.id = getNextID()
        self.e1 = e1
        self.e2 = e2
    def __repr__(self):
        return 'Mod(%s, %s)' % (self.e1, self.e2)
    def printme(self, depth):
        print ("  " * depth) + "Mod"
        self.e1.printme(depth + 1)
        self.e2.printme(depth + 1)
    def eval(self,x,y,t):
        (r1,g1,b1) = self.e1.eval(x,y,t)
        (r2,g2,b2) = self.e2.eval(x,y,t)
        try:
            r3 = r1 % r2
            g3 = g1 % g2
            b3 = b1 % b2
            return (r3, g3, b3)
        except:
            return (0,0,0)
    def getShader(self):
        substr = self.e1.getShader()
        substr += self.e2.getShader()
        substr += "vec3 val{0} = mod(val{1}, val{2});\n".format(self.id, self.e1.id, self.e2.id)
        return substr

class Well():
    arity = 1
    def __init__(self, e):
        self.id = getNextID()
        self.e = e
    def __repr__(self):
        return 'Well(%s)' % self.e
    def printme(self, depth):
        print ("  " * depth) + "Well"
        self.e.printme(depth + 1)
    def eval(self,x,y,t):
        (r,g,b) = self.e.eval(x,y,t)
        return (well(r), well(g), well(b))
    def getShader(self):
        substr = self.e.getShader()
        substr += "vec3 val{0} = 1.0 - (2.0 / (pow(1.0 + pow(val{1}, vec3(2.0)), vec3(8.0))));\n".format(self.id, self.e.id)
        return substr

class Tent():
    arity = 1
    def __init__(self, e):
        self.id = getNextID()
        self.e = e
    def __repr__(self):
        return 'Tent(%s)' % self.e
    def printme(self, depth):
        print ("  " * depth) + "Tent"
        self.e.printme(depth + 1)
    def eval(self,x,y,t):
        (r,g,b) = self.e.eval(x,y,t)
        return (tent(r), tent(g), tent(b))
    def getShader(self):
        substr = self.e.getShader()
        substr += "vec3 val{0} = 1.0 - (2.0 * abs(val{1}));\n".format(self.id, self.e.id)
        return substr

class Sin():
    arity = 1
    def __init__(self, e):
        self.id = getNextID()
        self.e = e
        self.phase = random.uniform(0, math.pi)
        self.freq = random.uniform(1.0, 6.0)
    def __repr__(self):
        return 'Sin(%g + %g * %s)' % (self.phase, self.freq, self.e)
    def printme(self, depth):
        print ("  " * depth) + "Sin"
        self.e.printme(depth + 1)
    def eval(self,x,y,t):
        (r1,g1,b1) = self.e.eval(x,y,t)
        r2 = math.sin(self.phase + self.freq * r1)
        g2 = math.sin(self.phase + self.freq * g1)
        b2 = math.sin(self.phase + self.freq * b1)
        return (r2,g2,b2)
    def getShader(self):
        substr = self.e.getShader()
        substr += "vec3 val{0} = sin({1} + ({2} * val{3}));\n".format(self.id, self.phase, self.freq, self.e.id)
        return substr

class Cos():
    arity = 1
    def __init__(self, e):
        self.id = getNextID()
        self.e = e
        self.phase = random.uniform(0, math.pi)
        self.freq = random.uniform(1.0, 6.0)
    def __repr__(self):
        return 'Cos(%g + %g * %s)' % (self.phase, self.freq, self.e)
    def printme(self, depth):
        print ("  " * depth) + "Cos"
        self.e.printme(depth + 1)
    def eval(self,x,y,t):
        (r1,g1,b1) = self.e.eval(x,y,t)
        r2 = math.cos(self.phase + self.freq * r1)
        g2 = math.cos(self.phase + self.freq * g1)
        b2 = math.cos(self.phase + self.freq * b1)
        return (r2,g2,b2)
    def getShader(self):
        substr = self.e.getShader()
        substr += "vec3 val{0} = cos({1} + ({2} * val{3}));\n".format(self.id, self.phase, self.freq, self.e.id)
        return substr

class Level():
    arity = 3
    def __init__(self, level, e1, e2):
        self.id = getNextID()
        self.treshold = random.uniform(-1.0,1.0)
        self.level = level
        self.e1 = e1
        self.e2 = e2
    def __repr__(self):
        return 'Level(%g, %s, %s, %s)' % (self.treshold, self.level, self.e1, self.e2)
    def printme(self, depth):
        print ("  " * depth) + "Level"
        self.level.printme(depth + 1)
        self.e1.printme(depth + 1)
        self.e2.printme(depth + 1)
    def eval(self,x,y,t):
        (r1, g1, b1) = self.level.eval(x,y,t)
        (r2, g2, b2) = self.e1.eval(x,y,t)
        (r3, g3, b3) = self.e2.eval(x,y,t)
        r4 = r2 if r1 < self.treshold else r3
        g4 = g2 if g1 < self.treshold else g3
        b4 = b2 if b1 < self.treshold else b3
        return (r4,g4,b4)

class Mix():
    arity = 3
    def __init__(self, w, e1, e2):
        self.id = getNextID()
        self.w = w
        self.e1 = e1
        self.e2 = e2
    def __repr__(self):
        return 'Mix(%s, %s, %s)' % (self.w, self.e1, self.e2)
    def printme(self, depth):
        print ("  " * depth) + "Mix"
        self.w.printme(depth + 1)
        self.e1.printme(depth + 1)
        self.e2.printme(depth + 1)
    def eval(self,x,y,t):
        w = 0.5 * (self.w.eval(x,y,t)[0] + 1.0)
        c1 = self.e1.eval(x,y,t)
        c2 = self.e2.eval(x,y,t)
        return average(c1,c2,)
    def getShader(self):
        #dont use w since it is not actually used?
        substr = self.e1.getShader()
        substr += self.e2.getShader()
        substr += "vec3 val{0} = (val{1} + val{2}) / 2.0;\n".format(self.id, self.e1.id, self.e2.id)
        return substr

# The following list of all classes that are used for generation of expressions is
# used by the generate function below.

#remove level because its a pain to write LOL
#operators = (VariableX, VariableY, VariableT, Constant, Sum, Product, Mod, Sin, Cos, Tent, Well, Level, Mix)
operators = (VariableX, VariableY, VariableT, Constant, Sum, Product, Mod, Sin, Cos, Tent, Well, Mix)

# We precompute those operators that have arity 0 and arity > 0

operators0 = [op for op in operators if op.arity == 0]
operators1 = [op for op in operators if op.arity > 0]

def generate(k = 50):
    '''Randonly generate an expession of a given size.'''
    if k <= 0: 
        # We used up available size, generate a leaf of the expression tree
        op = random.choice(operators0)
        return op()
    else:
        # randomly pick an operator whose arity > 0
        op = random.choice(operators1)
        # generate subexpressions
        i = 0 # the amount of available size used up so far
        args = [] # the list of generated subexpression
        for j in sorted([random.randrange(k) for l in range(op.arity-1)]):
            args.append(generate(j - i))
            i = j
        args.append(generate(k - 1 - i))
        return op(*args)

class Art():
    """A simple graphical user interface for random art. It displays the image,
       and the 'Again!' button."""

    def __init__(self, seed=None, size=4096):
        self.size=size
        self.seed = seed

    def MakeArt(self):
        if self.seed is not None:
            random.seed(self.seed)
        xpixels = int(math.sqrt(self.size))
        ypixels = xpixels
        frames = 256
        art = generate(random.randrange(20,150))
        #art.printme(0)
        outstr = art.getShader()
        outstr += "gl_FragColor.rgb = val{0};".format(art.id)
        outstr += "gl_FragColor.a = 1.0;"
        return outstr

# Main program
if __name__ == "__main__":
    art = Art(512*512)
    print art.MakeArt()
