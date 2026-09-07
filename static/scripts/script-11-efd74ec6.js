
      const goatcounterScriptPre = document.createElement('script');
      goatcounterScriptPre.textContent = `
        window.goatcounter = { no_onload: true };
      `;
      document.head.appendChild(goatcounterScriptPre);

      const endpoint = "https://flizamar.goatcounter.com/count";
      const goatcounterScript = document.createElement('script');
      goatcounterScript.src = "https://gc.zgo.at/count.js";
      goatcounterScript.defer = true;
      goatcounterScript.setAttribute('data-goatcounter', endpoint);
      goatcounterScript.onload = () => {
        window.goatcounter.endpoint = endpoint;
        // Patch local: interruptor de auto-exclusion — visitar cualquier URL
        // del sitio con #toggle-goatcounter alterna si ESTE navegador cuenta.
        try {
          if (location.hash === '#toggle-goatcounter') {
            if (localStorage.getItem('skipgc') === 't') {
              localStorage.removeItem('skipgc');
              alert('GoatCounter: tus visitas desde este navegador VOLVERAN a contarse.');
            } else {
              localStorage.setItem('skipgc', 't');
              alert('GoatCounter: tus visitas desde este navegador YA NO se contaran.');
            }
          }
        } catch (e) { /* localStorage bloqueado: seguir contando */ }
        // Patch local: el evento 'nav' del SPA tambien se dispara en la carga
        // inicial, lo que duplicaba el primer pageview; deduplicar por ruta.
        let goatcounterLastPath = null;
        const goatcounterRecord = () => {
          try { if (localStorage.getItem('skipgc') === 't') return; } catch (e) {}
          if (location.pathname === goatcounterLastPath) return;
          goatcounterLastPath = location.pathname;
          goatcounter.count({ path: location.pathname });
        };
        goatcounterRecord();
        document.addEventListener('nav', goatcounterRecord);
      };

      document.head.appendChild(goatcounterScript);
    