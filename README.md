## RGBA-BMP转换工具（node c++扩展）

### About

高效率的nodejs图片rgba数据转bmp工具，由于单线程的js不适合cpu密集型的计算业务，纯前端转换性能低下，以1000*1000的图片为例，转换耗时往往需要几百毫秒，本插件只需要几毫秒

### Usage

```
// electron demo

const BMPTool = require('./build/Release/index.node');

const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');

...

const imageData = ctx.getImageData(0,0, width,height);

const bmpData = BMPTool.rgbaToBmp(new Buffer(imageData.data), imageData.width, imageData.height);

console.log(bmpData); // bmp图像buffer


```