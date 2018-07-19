const fs = require('fs');
const addon = require('./build/Release/index.node');

function createRGBData(width, height) {
    let imageData = [];
    let idx = 0;
    for (let i = 0; i < width * height * 4; i = i + 4) {
        let r, g, b, a;
        if (i < width * height * 4 / 2) {
            r = 0;
            g = 0;
            b = 0;
            a = 255;
        } else {
            r = 255;
            g = 0;
            b = 0;
            a = 255;
        }
        imageData[i] = r;
        imageData[i + 1] = g;
        imageData[i + 2] = b;
        imageData[i + 3] = a;
    }

    return imageData;
}

const imageData = createRGBData(1280, 1024);
const buff = new Buffer(imageData);
console.time("转换耗时");
const bmpData = addon.rgbaToBmp(buff, 1280, 1024);
console.timeEnd("转换耗时");
fs.writeFileSync('c:\\hello.bmp', bmpData);

