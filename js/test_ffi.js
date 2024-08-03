import { CFunction, freeCif } from './quickjs-ffi.js'
var LIBC_SO = 'msvcrt.dll';
let printf = new CFunction(LIBC_SO, 'printf', 1, 'int', 'string', 'double', 'double', 'int');
printf.invoke('test count: %g %g %d\n', 3.141592654, 2.718281829, 299792458);
freeCif(printf.cifcacheindex);
printf.free();
printf = new CFunction(LIBC_SO, 'printf', 1, 'int', 'string', 'string', 'string');
printf.invoke('test2: %s %s\n', 'hello', 'world');
freeCif(printf.cifcacheindex);
printf.free();
