var util = require('util');
var Event = require('./base');


function KeyboardEvent(type, obj) {
  Event.call(this, type, obj);

  Object.defineProperty(this, 'keyCode', {
    value: obj.keyCode
  });

  Object.defineProperty(this, 'ctrlKey', {
    value: obj.keyCode
  });

  Object.defineProperty(this, 'shiftKey', {
    value: obj.keyCode
  });

  Object.defineProperty(this, 'altKey', {
    value: obj.keyCode
  });

  Object.defineProperty(this, 'metaKey', {
    value: obj.keyCode
  });

};

KeyboardEvent.prototype = {
  getModifierState : function() {
    throw new Error('not implemented... yet');
  }
};


util.inherits(Event, KeyboardEvent);

module.exports = KeyboardEvent;
