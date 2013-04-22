
function Event(type, obj) {
  var bubbles = false, cancelable = false;


  Object.defineProperty(this, 'type', {
    get : function() {
      return type;
    },
    configurable : false
  });

  Object.defineProperty(this, 'target', {
    get : function() {
      return obj.target;
    },
    configurable : false
  });

  Object.defineProperty(this, 'currentTarget', {
    get : function() {
      return obj.target;
    },
    configurable : false
  });


  var time = Date.now();
  Object.defineProperty(this, 'timeStamp', {
    get : function() {
      return time;
    },
    configurable : false
  });


  Object.defineProperty(this, 'eventPhase', {
    get : function() {
      return this.AT_TARGET;
    },
    configurable : false
  });


  Object.defineProperty(this, 'defaultPrevented', {
    get : function() {
      return obj._defaultPrevented;
    },
    configurable : false
  });


  this.preventDefault = function() {
    console.log('default prevented');
    obj._defaultPrevented = true;
  };


  Object.defineProperty(this, 'bubbles', {
    get : function() {
      return bubbles;
    },
    configurable : false
  });


  Object.defineProperty(this, 'cancelable', {
    get : function() {
      return cancelable;
    },
    configurable : false
  });


  Object.defineProperty(this, 'isTrusted', {
    get : function() {
      return true;
    },
    configurable : false
  });
}


Event.prototype = {
  initEvent : function() {},
  stopPropagation : function() {},
  stopImmediatePropagation : function() {},

  NONE : 0,
  CAPTURING_PHASE : 1,
  AT_TARGET : 2,
  BUBBLING_PHASE : 3
};


module.exports = Event;