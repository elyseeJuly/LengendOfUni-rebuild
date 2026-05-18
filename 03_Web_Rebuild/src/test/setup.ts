import '@testing-library/jest-dom';

// Mock browser APIs for headless testing
(globalThis as any).window = globalThis.window || {
  dispatchEvent: () => true,
  addEventListener: () => {},
  removeEventListener: () => {},
  CustomEvent: class { 
    type: string; 
    detail: any; 
    constructor(t: string, o?: any) { 
      this.type = t; 
      this.detail = o?.detail; 
    } 
  },
  localStorage: { 
    getItem: () => null, 
    setItem: () => {}, 
    removeItem: () => {} 
  },
};
