import React, { useEffect, useRef } from 'react';
import { StarMapRenderer } from '../ui/StarMapRenderer';
import { GameInstance } from '../core/Game';

export const StarMap: React.FC = () => {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const rendererRef = useRef<StarMapRenderer | null>(null);

  useEffect(() => {
    if (canvasRef.current) {
      // Initialize the existing renderer
      // Note: StarMapRenderer expects an ID, but we can modify it or just give it one
      const canvasId = "star-canvas-main";
      canvasRef.current.id = canvasId;
      
      try {
        const renderer = new StarMapRenderer(canvasId);
        rendererRef.current = renderer;
        
        // Load data from game instance
        const game = GameInstance.get();
        renderer.initStars(game.starManager.getAllStars());
        
        // Start animation
        renderer.start();
        
        // Cleanup on unmount
        return () => {
          renderer.stop();
        };
      } catch (err) {
        console.error("StarMapRenderer failed to init:", err);
      }
    }
  }, []);

  return (
    <div className="w-full h-full relative group">
      <canvas 
        ref={canvasRef} 
        className="w-full h-full block cursor-crosshair transition-opacity duration-700"
      />
      
      {/* Overlay for map controls */}
      <div className="absolute bottom-6 left-1/2 -translate-x-1/2 flex items-center gap-4 px-6 py-2 glass-panel rounded-full border border-white/10 opacity-0 group-hover:opacity-100 transition-opacity duration-300">
        <div className="text-[10px] font-bold text-[var(--text-secondary)] uppercase tracking-[0.2em] whitespace-nowrap">
          Star Map Controls
        </div>
        <div className="h-4 w-px bg-white/10" />
        <div className="flex gap-4">
            <button className="text-[10px] hover:text-[var(--color-primary)] transition-colors uppercase font-bold">Zoom In</button>
            <button className="text-[10px] hover:text-[var(--color-primary)] transition-colors uppercase font-bold">Zoom Out</button>
            <button className="text-[10px] hover:text-[var(--color-primary)] transition-colors uppercase font-bold">Reset View</button>
        </div>
      </div>

      {/* Decorative corners */}
      <div className="absolute top-4 left-4 w-8 h-8 border-t-2 border-l-2 border-white/10" />
      <div className="absolute top-4 right-4 w-8 h-8 border-t-2 border-r-2 border-white/10" />
      <div className="absolute bottom-4 left-4 w-8 h-8 border-b-2 border-l-2 border-white/10" />
      <div className="absolute bottom-4 right-4 w-8 h-8 border-b-2 border-r-2 border-white/10" />
    </div>
  );
};
