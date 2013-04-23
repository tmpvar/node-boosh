var util = require('util');
var Event = require('./base');


function MouseEvent(type, obj) {
  Event.call(this, type, obj);

  Object.defineProperty(this, 'clientX', {
    value : obj.x
  });

  Object.defineProperty(this, 'clientY', {
    value : obj.y
  });

  Object.defineProperty(this, 'screenX', {
    get : function() {
      if (obj.target && obj.target._window) {
        var rect = obj.target._window.getRect();
        if (rect) {
          return rect.x + obj.x;
        }
      }
    }
  });

  Object.defineProperty(this, 'screenY', {
    get : function() {
      if (obj.target && obj.target._window) {
        var rect = obj.target._window.getRect();
        if (rect) {
          return rect.y + obj.y;
        }
      }
    }
  });

  Object.defineProperty(this, 'button', {
    value : obj.button || 0
  });

}

MouseEvent.prototype = {
  getModifierState : function() {
    throw new Error('not implemented... yet');
  }
}



util.inherits(Event, MouseEvent);

module.exports = MouseEvent;
