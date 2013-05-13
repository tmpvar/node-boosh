var util = require('util');
var EventEmitter = require('events').EventEmitter;
var binding = require('bindings')('boosh');
var context2d = require('context2d');
var fs = require('fs');
var NativeWindow = binding.Window;
var AnimationFrame = require('animationframe');

var events = require('./lib/event');

function Window(options) {
  options = options || {};

  options.width = options.width || 640;
  options.height = options.height || 480;
  options.title = options.title || 'boosh!';
  options.fullscreen = options.fullscreen || false;

  EventEmitter.call(this);
  this._window = new NativeWindow(options.width, options.height, options.title);

  if (!this._window) {
    return;
  }

  // Event dispatcher
  this._window.eventHandler(function(raw) {
    raw.target = this;
    var objectType = raw.objectType || 'Event';
    var ev = new (events[objectType])(raw.type, raw);

    // window.on<event> handlers
    var lower = raw.type.toLowerCase();

    if (typeof this['on' + lower] === 'function') {
      this[lower](ev);
    }

    this.emit(raw.type, ev);

    if (raw.type === 'close' && !ev.defaultPrevented) {
      this.close();
    }

  }.bind(this));

  this.addEventListener = this.on;
  this.removeEventListener = this.removeListener;

  Object.defineProperty(this, 'title', {
    set: function(v) {
      title = v;
      return this._window.setTitle(v);
    },
    get : function() {
      return title;
    },
    configurable: false
  });

  var manager = new AnimationFrame(function() {
    this._window.flush();
  }.bind(this));

  this.requestAnimationFrame = manager.requestAnimationFrame;
  this.cancelAnimationFrame = manager.cancelAnimationFrame;

  this.close = function() {
    this._window.close();
    delete this._window;
    manager.destroy();
  };
}

util.inherits(Window, EventEmitter);

var windowPrototypeProperty = function(properties, get, set) {

  if (!Array.isArray(properties)) {
    properties = [properties];
  }

  var definition = {
    configurable : false
  };

  if (get) {
    definition.get = get;
  }


  if (set) {
    definition.set = set;
  }

  properties.forEach(function(name) {
    Object.defineProperty(Window.prototype, name, definition);
  });

};

windowPrototypeProperty(['outerWidth', 'innerWidth'], function() {
  var rect = this._window.getRect();
  if (rect) {
    return rect.width;
  }
});

windowPrototypeProperty(['outerHeight', 'innerHeight'], function() {
  var rect = this._window.getRect();
  if (rect) {
    return rect.height;
  }
});


windowPrototypeProperty(['screenTop', 'screenX'], function() {
  var rect = this._window.getRect();
  if (rect) {
    return rect.x;
  }
},
function(v) {
  var rect = this._window.getRect();
  if (rect) {
    this.moveTo(v, rect.y);
  }
});

windowPrototypeProperty(['screenLeft', 'screenY'], function() {
  var rect = this._window.getRect();
  if (rect) {
    return rect.y;
  }
},
function(v) {
  var rect = this._window.getRect();
  if (rect) {
    this.moveTo(rect.x, v);
  }
});

Window.prototype.resizeTo = function(w, h) {
  w = w || 0;
  h = h || 0;
  this._window.resizeTo(w, h);
};

Window.prototype.resizeBy = function(w, h) {
  var rect = this._window.getRect();
  if (rect) {
    this.resizeTo(rect.width + w, rect.height + h);
  }
};

Window.prototype.moveTo = function(x, y) {
  x = x || 0;
  y = y || 0;
  this._window.moveTo(x, y);
};

Window.prototype.moveBy = function(x, y) {
  var rect = this._window.getRect();
  if (rect) {
    this.moveTo(rect.x + x, rect.y + y);
  }
};

Window.prototype.blur = Window.prototype.focus = function() {
  throw new Error('DERP - stealing focus isn\'t cool');
}

Window.prototype.getContext = function(type) {
  if (this.context) {
    throw new Error('context already created');
  }

  if (type === '2d') {
    this.context = context2d.createContext(this, this.width, this.height, binding.Context2D);
    this._window.setContext2d(this.context);
    return this.context;
  }
  this.context = this.canvas.getContext(type);
  return this.context;
};

/*

  options:
    width : <int>
    height: <int>
    fullscreen: <int>

*/

module.exports.createWindow = function(options) {
  var ret = new Window(options);

  if (!ret) {
    throw new Error('window could not be created');
  }
  return ret;
};



var context = {};

context.window = module.exports.createWindow({
  width : Math.floor(Math.random() * 640) + 200,
  height: Math.floor(Math.random() * 480) + 200
});

context.closeAttempts = 0;
context.window.addEventListener('close', function(ev) {
  context.closeAttempts++;
  if (context.closeAttempts <= 1) {
    ev.preventDefault();
  }
});

context.color = '#000'

context.window.addEventListener('blur', function() {
  context.color = "#ddd";
});

context.window.addEventListener('focus', function() {
  context.color = "#000";
});

context.ctx = context.window.getContext('2d');
context.x = 0;
context.xv = 1;
context.y = 0;
context.yv = 1;


context.mouse = { x: 0, y: 0, color: 'red'};
context.window.addEventListener('mousemove', function(ev) {
  var x = ev.clientX, y = ev.clientY;
  if (x + 20 > context.window.innerWidth) {
    x = context.window.innerWidth-20;
  } else if (x < 20) {
    x = 20;
  }

  if (y + 20 > context.window.outerHeight) {
    y = context.window.innerHeight-20;
  } else if (y < 20) {
    y = 20;
  }

  context.mouse.x = x;
  context.mouse.y = y;
});

context.window.addEventListener('mouseenter', function(ev) {
  context.mouse.color = "red";
});

context.window.addEventListener('mouseleave', function(ev) {
  context.mouse.color = "blue";
});

context.window.addEventListener('mousedown', function(ev) {
  var buttons = ['green', 'orange', 'purple', 'magenta', 'pink'];
  context.mouse.color = buttons[ev.button];
});

context.window.addEventListener('mouseup', function(ev) {
  context.mouse.color = "red";
});


context.text = {
  value : '',
  color : 'black'
};

context.window.addEventListener('keydown', function(ev) {

  if (ev.keyCode === 27) {
    context.window.close();
  } else {
    context.text.value += String.fromCharCode(ev.keyCode);
  }
});


context.window.requestAnimationFrame(function() {
  context.ctx.fillStyle = '#aaa';
  context.ctx.fillRect(0,0,context.window.innerWidth,context.window.innerHeight)

  context.ctx.fillStyle = context.color;
  context.x+=context.xv;
  context.y+=context.yv;

  context.window.title = context.x + ',' + context.y;

  context.ctx.save();
    context.ctx.translate(context.x, context.y);
    context.ctx.fillRect(0, 0, 100, 100);
  context.ctx.restore();

  context.ctx.fillStyle=context.mouse.color;
  context.ctx.fillRect(context.mouse.x - 20, context.mouse.y - 20, 40, 40);

  if (context.y > context.window.innerHeight-100) {
    context.yv = -Math.abs(context.yv);
  } else if (context.y < 0) {
    context.yv = Math.abs(context.yv)
  }

  if (context.x > context.window.innerWidth-100) {
    context.xv = -Math.abs(context.xv);
  } else if (context.x < 0) {
    context.xv = Math.abs(context.xv)
  }

  context.ctx.font="20px sans-serif";
  context.ctx.fillStyle = context.text.color;
  context.ctx.fillText(context.text.value, 5, 30);
});
