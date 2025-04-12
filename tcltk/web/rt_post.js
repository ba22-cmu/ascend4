(async function main(){

// If MODULARIZE is used, `createModule` is a promise.
if (createModule instanceof Promise) {
  createModule = await createModule;
}

var r1 = new createModule.rt();
var r2 = new createModule.rt("a", 1, 2);
x = r1.rc;
y = r2.s;
z = r2.t;
console.log(x);
console.log(y);
console.log(r1);

})
