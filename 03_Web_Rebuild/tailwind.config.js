/** @type {import('tailwindcss').Config} */
export default {
  content: [
    "./index.html",
    "./src/**/*.{js,ts,jsx,tsx}",
  ],
  darkMode: 'class',
  theme: {
    extend: {
      colors: {
        // Theme A: United Government Bridge (Light)
        'gov-bg': '#F0F4F8',
        'gov-panel': 'rgba(255, 255, 255, 0.9)',
        'gov-primary': '#0D47A1',
        'gov-secondary': '#00B8D4',
        'gov-danger': '#E65100',
        'gov-neutral': '#9E9E9E',
        'gov-text': '#212121',
        'gov-text-muted': '#757575',

        // Theme B: Dark Forest (Dark)
        'forest-bg': '#050A1F',
        'forest-panel': 'rgba(10, 17, 40, 0.85)',
        'forest-primary': '#00E5FF',
        'forest-danger': '#FF5722',
        'forest-text': '#E2E8F0',
        'forest-text-muted': '#94A3B8',
        'forest-border': 'rgba(255, 255, 255, 0.1)',
        'forest-active-bg': 'rgba(0, 229, 255, 0.1)',
      },
      fontFamily: {
        base: ['Inter', 'system-ui', 'sans-serif'],
        data: ['Roboto Mono', 'JetBrains Mono', 'monospace'],
      },
      backdropBlur: {
        xs: '2px',
      }
    },
  },
  plugins: [],
}
