var util = require('util');
var EventEmitter = require('events').EventEmitter;
var binding = require('bindings')('boosh');
var context2d = require('context2d');
var fs = require('fs');
var NativeWindow = binding.Window;
var AnimationFrame = require('animationframe');

var events = require('./lib/event');

function Window(options) {
  EventEmitter.call(this);

  options = options || {};
  var title = options.title = options.title || 'boosh';

  this.addEventListener = this.on;
  this.removeEventListener = this.removeListener;

  this._createNativeWindow(options);


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
    if (this.context && this.context.dirty) {
      this._window.flush();
    }
  }.bind(this));

  this.requestAnimationFrame = manager.requestAnimationFrame;
  this.cancelAnimationFrame = manager.cancelAnimationFrame;

  this.close = function() {
    this._window.close();
    delete this._window;
    delete this.context;
    manager.destroy();
  };
}

util.inherits(Window, EventEmitter);

Window.prototype._createNativeWindow = function(options) {
  options = options || {};

  options.width = options.width || 640;
  options.height = options.height || 480;
  options.title = options.title || 'boosh!';
  options.fullscreen = options.fullscreen || false;

  this._window = new NativeWindow(
    options.width,
    options.height,
    options.title,
    options.fullscreen
  );

  if (!this._window) {
    return;
  }

  // re-setup the context
  if (this.context) {
    this.context.width = this.width;
    this.context.height = this.height;
    this._window.setContext2d(this.context);
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
}

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

Window.prototype.fullScreen = false;
Window.prototype.requestFullScreen = function() {
  var win = this._window;
  this._createNativeWindow({
    width : this.outerWidth,
    height: this.outerHeight,
    title: this.title,
    fullscreen: true
  });
  this.fullScreen = true;

  win.close();
};

Window.prototype.cancelFullScreen = function() {
  var win = this._window;

  this._createNativeWindow({
    width: 320,
    height: 200,
    title: 'asdf',
    fullscreen: false
  });
  this.fullScreen = false;
  win.close();
}

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

