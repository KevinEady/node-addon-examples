var { AddonInstance } = require('bindings')('hello');

function foo(arg0, arg1) {
    console.log('bar', arg0, arg1);
}
const inst = new AddonInstance;
inst.exportedFunction(foo);
console.log(inst); // 'bar'
