var util = require('util');
var EventEmitter = require('events').EventEmitter;
var binding = require('bindings')('boosh');
var Canvas = require('canvas');
var defaults = require('defaults');
var fs = require('fs');
var NativeWindow = binding.Window;

var Event = require('./lib/event');

function Window(options) {
  console.log(options);
  options = defaults(options || {}, {
    width: 640,
    height: 480,
    title: 'boosh!',
    fullscreen: false,
  });


  EventEmitter.call(this);
  this._window = new NativeWindow(options.width, options.height, options.title);

  if (!this._window) {
    return;
  }

  // Event dispatcher
  this._window.eventHandler(function(raw) {
    raw.target = this;
    var ev = new Event(raw.type, raw);

    // window.on<event> handlers
    var lower = raw.type.toLowerCase();

    if (typeof this['on' + lower] === 'function') {
      this[lower](ev);
    }

    this.emit(raw.type, ev);
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


  // this.width = options.width;
  // this.height = options.height;

  this.canvas = new Canvas(options.width, options.height);
}

util.inherits(Window, EventEmitter);

// TODO: move this into requestAnimationFrame
Window.prototype.flush = function() {
  this._window.flush();
};

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

Window.prototype.close = function() {
  this._window.close();
  delete this;
};

Window.prototype.blur = Window.prototype.focus = function() {
  throw new Error('DERP - stealing focus isn\'t cool');
}


Window.prototype.getContext = function(type) {
  if (this.context) {
    throw new Error('context already created');
  }

  if (type === '2d') {
    this.context = this.canvas.getContext(type);
    this._window.get2dContext(this.canvas);
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


var contexts = [];

for (var i=0; i<1; i++) {
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
    } else {
      contexts.shift();
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

  contexts.push(context);
}

var timer = setTimeout(function tick() {
  contexts.forEach(function(context) {

    context.ctx.fillStyle = '#aaa';//context.color;
    context.ctx.fillRect(0,0,context.window.innerWidth,context.window.innerHeight)

    context.ctx.fillStyle = context.color;
    context.x+=context.xv;
    context.y+=context.yv;

    context.window.title = context.x + ',' + context.y;

    context.ctx.save();
      context.ctx.translate(context.x, context.y);
      context.ctx.fillRect(0, 0, 100, 100);
    context.ctx.restore();

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

    context.window.flush();
  });
  contexts.length && process.nextTick(tick);
}, 1000/40);

