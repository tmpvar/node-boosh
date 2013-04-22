var util = require('util');
var EventEmitter = require('events').EventEmitter;
var binding = require('bindings')('boosh');
var Canvas = require('canvas');
var defaults = require('defaults');
var fs = require('fs');
var NativeWindow = binding.Window;

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

  this._window.eventHandler(function(raw) {
    raw.preventDefault = function() {
      raw._defaultPrevented = true;
    };

    // window.on<event> handlers
    var lower = raw.type.toLowerCase();

    if (typeof this['on' + lower] === 'function') {
      this[lower](raw);
    }

    this.emit(raw.type, raw);
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

Window.prototype.flush = function() {
  this._window.flush();
};

Object.defineProperty(Window.prototype, 'width', {
  get: function() {
    var rect = this._window.getRect();
    if (rect) {
      return rect.width;
    }
  },
  configurable: false
});

Object.defineProperty(Window.prototype, 'height', {
  get: function() {
    var rect = this._window.getRect();
    if (rect) {
      return rect.height;
    }
  },
  configurable: false
});


Window.prototype.close = function() {
  this._window.close();
  delete this;
};


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
    width : Math.floor(Math.random() * 640),
    height: Math.floor(Math.random() * 480)
  });

  context.closeAttempts = 0;
  context.window.addEventListener('close', function(ev) {
    context.closeAttempts++;
    console.log('CLOSE!', context.closeAttempts)
    if (context.closeAttempts <= 1) {
      ev.preventDefault();
    } else {
      contexts.shift();
    }
  });

  context.ctx = context.window.getContext('2d');
  context.x = 0;
  context.xv = 1;
  context.y = 0;
  context.yv = 1;

  context.color = 'rgba(' + [
    Math.floor(Math.random()*255),
    Math.floor(Math.random()*255),
    Math.floor(Math.random()*255),
    1
  ].join(',') + ')';

  contexts.push(context);
}

var timer = setTimeout(function tick() {
  contexts.forEach(function(context) {
    context.ctx.fillStyle = '#aaa';//context.color;
    context.ctx.fillRect(0,0,context.window.width,context.window.height)

    context.ctx.fillStyle = '#000';//context.color;
    context.x+=context.xv;
    context.y+=context.yv;

    context.window.title = context.x + ',' + context.y;

    context.ctx.save();
      context.ctx.translate(context.x, context.y);
      context.ctx.fillRect(0, 0, 100, 100);
    context.ctx.restore();

    if (context.y > context.window.height-100) {
      context.yv = -Math.abs(context.yv);
    } else if (context.y < 0) {
      context.yv = Math.abs(context.yv)
    }

    if (context.x > context.window.width-100) {
      context.xv = -Math.abs(context.xv);
    } else if (context.x < 0) {
      context.xv = Math.abs(context.xv)
    }

    context.window.flush();
  });
  contexts.length && process.nextTick(tick);
}, 1000/40);

