import React, { useState, useEffect } from 'react';
import { GameInstance } from '../core/Game';
import { Radio } from 'lucide-react';

export const NewsTicker: React.FC = () => {
  const [messages, setMessages] = useState<string[]>([]);

  const updateMessages = () => {
    const game = GameInstance.get();
    if (game && game.tickerMessages) {
      setMessages([...game.tickerMessages]);
    }
  };

  useEffect(() => {
    updateMessages();

    window.addEventListener('ticker-message-added', updateMessages);
    window.addEventListener('game-turn-complete', updateMessages);
    window.addEventListener('game-loaded', updateMessages);

    return () => {
      window.removeEventListener('ticker-message-added', updateMessages);
      window.removeEventListener('game-turn-complete', updateMessages);
      window.removeEventListener('game-loaded', updateMessages);
    };
  }, []);

  if (messages.length === 0) {
    return (
      <div className="w-full bg-blue-950/10 border-y border-cyan-500/10 h-8 flex items-center px-4 overflow-hidden select-none">
        <div className="flex items-center text-cyan-400/50 text-[11px] font-mono tracking-widest gap-2">
          <Radio className="w-3.5 h-3.5 animate-pulse text-cyan-400/70" />
          <span>[ PDC DEEP SPACE BROADCAST ]</span>
          <span className="text-cyan-400/30">STANDBY - MONITORING TRISOLARAN FLEET TRAJECTORY...</span>
        </div>
      </div>
    );
  }

  // Show the last 3 messages scrolling continuously, joined by sci-fi separators
  const displayText = messages.slice(-3).reverse().join("   <<<   ");

  return (
    <div className="w-full bg-cyan-950/30 backdrop-blur-md border-y border-cyan-500/20 h-9 flex items-center px-4 overflow-hidden select-none relative z-10 shadow-[inset_0_0_10px_rgba(6,182,212,0.1)]">
      {/* Ticker Title Badge */}
      <div className="flex items-center text-cyan-400 text-[11px] font-bold font-mono tracking-wider gap-2 bg-cyan-500/10 border border-cyan-500/30 px-2 py-0.5 rounded shadow-[0_0_8px_rgba(6,182,212,0.2)] mr-4 shrink-0 animate-pulse">
        <Radio className="w-3.5 h-3.5 text-cyan-400" />
        <span>PDC 战略广播</span>
      </div>

      {/* Scrolling Text Window */}
      <div className="flex-1 overflow-hidden relative h-full flex items-center">
        <div className="animate-marquee whitespace-nowrap text-cyan-200/90 text-xs font-mono tracking-wide flex items-center gap-4">
          <span>{displayText}</span>
        </div>
      </div>

      {/* Embedded CSS for smooth marquee scrolling and hovering interactions */}
      <style>{`
        @keyframes marquee {
          0% { transform: translateX(80vw); }
          100% { transform: translateX(-100%); }
        }
        .animate-marquee {
          animation: marquee 25s linear infinite;
        }
        .animate-marquee:hover {
          animation-play-state: paused;
        }
      `}</style>
    </div>
  );
};
