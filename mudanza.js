(function () {
  var prefijo = '/entrenamiento-ioi-web';
  var ruta = location.pathname.indexOf(prefijo) === 0 ? location.pathname.slice(prefijo.length) : "/";
  ruta = ruta.replace(/(^|\/)index\.html$/, "$1").replace(/\.html$/, "");
  location.replace('https://ioi.felipedia.cl' + ruta + location.search + location.hash);
})();
