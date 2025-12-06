# Bienestar+ 🏥

**Sistema de Telemedicina y Monitoreo IoT Preventivo**

Bienestar+ es un ecosistema de salud digital diseñado para transformar el
cuidado de enfermedades crónicas de un modelo reactivo a uno **proactivo y
preventivo**. Utilizando dispositivos wearables comerciales (smartwatches) y una
plataforma en la nube, permitimos el monitoreo continuo de signos vitales para
la detección temprana de anomalías.

---

## Objetivo

Empoderar al paciente y facilitar la intervención médica temprana. El sistema
busca reducir hospitalizaciones y riesgos fatales (como caídas o arritmias)
mediante el análisis de datos 24/7, sin la necesidad de hardware médico costoso
y especializado.

## FUnciones Principales

- **Monitoreo 24/7:** Seguimiento continuo de ritmo cardíaco, SpO2 y temperatura
  corporal mediante wearables de consumo (vía Bluetooth).
- **Detección de Anomalías:** Algoritmos en la nube que identifican patrones
  irregulares (arritmias, fiebre, caídas).
- **Alertas Inmediatas:** Notificaciones automáticas al Portal Médico y
  familiares en caso de emergencia (< 60 segundos).
- **Accesibilidad:** Diseñado para funcionar con hardware comercial para reducir
  costos de entrada.
- **Soporte Multi-idioma:** Interfaz internacionalizada (Español, Inglés,
  Alemán) usando _Inlang Paraglide_.

## Stack (tecnología utilizada)

Este repositorio contiene el frontend y la lógica de presentación del sistema:

- **Framework:** [SvelteKit](https://kit.svelte.dev/) (Svelte 5)
- **Estilos:** [TailwindCSS v4](https://tailwindcss.com/) +
  [DaisyUI](https://daisyui.com/)
- **Lenguaje:** TypeScript
- **Internacionalización (i18n):** [Inlang Paraglide JS](https://inlang.com/)
- **Empaquetador:** Vite

## Instalación y Ejecución

Para ejecutar el proyecto localmente:

1. **Instalar dependencias:**
   ```sh
   deno install
   ```

2. **Iniciar servidor de desarrollo:**
   ```sh
   deno run --allow-net --allow-read server.ts
   ```

3. **Iniciar la aplicación SvelteKit:**
   ```sh
   # o abrir automáticamente en el navegador
   deno task dev --open
   ```

## ⚠️ Aviso Importante

**EL SISTEMA ES ÚNICAMENTE DE MONITOREO.** Bienestar+ es una herramienta de
apoyo tecnológico. No somos médicos ni sustituimos el diagnóstico clínico
profesional. Los datos recolectados sirven para alertar y prevenir, pero
cualquier decisión médica debe ser tomada por profesionales de la salud.

---

_Facultad de Ingeniería de UAEM - Proyecto IoT Bienestar+_
