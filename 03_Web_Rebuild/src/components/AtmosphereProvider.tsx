/**
 * AtmosphereProvider - 全局氛围 Provider
 *
 * 监听 AtmosphereEngine 状态变化，通过 React Context 向下传递配色配置。
 * CSS 变量和背景叠层实现全局视觉氛围切换。
 */

import React, { createContext, useContext, useEffect, useState, useRef } from 'react';
import { AtmosphereConfig, AtmosphereState } from '../core/AtmosphereEngine';

const AtmosphereContext = createContext<AtmosphereConfig | null>(null);

export const useAtmosphere = () => useContext(AtmosphereContext);

interface Props {
  engineRef: { current: { getConfig: () => AtmosphereConfig; evaluate: (tm: any, ec: any) => AtmosphereState } | null };
  children: React.ReactNode;
}

export const AtmosphereProvider: React.FC<Props> = ({ engineRef, children }) => {
  const [config, setConfig] = useState<AtmosphereConfig>({
    state: 'NORMAL',
    backgroundColor: '#0a0a1a',
    uiTint: 'rgba(0, 229, 255, 0.05)',
    noiseLevel: 0.02,
    scanlineOpacity: 0.03,
    vignetteIntensity: 0.1,
    textGlowColor: 'rgba(0, 229, 255, 0.3)',
    transitionMs: 2000,
    label: '正常',
    description: '人类文明处于相对稳定状态',
  });
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const animFrameRef = useRef<number>(0);

  useEffect(() => {
    const handler = () => {
      const eng = engineRef.current;
      if (eng) {
        setConfig(eng.getConfig());
      }
    };

    window.addEventListener('game:atmosphere:changed', handler);
    return () => window.removeEventListener('game:atmosphere:changed', handler);
  }, [engineRef]);

  // 背景噪点 + 扫描线效果（Canvas）
  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const ctx = canvas.getContext('2d');
    if (!ctx) return;

    const resize = () => {
      canvas.width = window.innerWidth;
      canvas.height = window.innerHeight;
    };
    resize();
    window.addEventListener('resize', resize);

    const draw = () => {
      ctx.clearRect(0, 0, canvas.width, canvas.height);

      // 噪点
      if (config.noiseLevel > 0) {
        const imageData = ctx.createImageData(canvas.width, canvas.height);
        const data = imageData.data;
        for (let i = 0; i < data.length; i += 4) {
          if (Math.random() < config.noiseLevel) {
            const val = Math.random() * 255;
            data[i] = val;
            data[i + 1] = val;
            data[i + 2] = val;
            data[i + 3] = 30;
          }
        }
        ctx.putImageData(imageData, 0, 0);
      }

      // 扫描线
      if (config.scanlineOpacity > 0) {
        ctx.fillStyle = `rgba(0, 0, 0, ${config.scanlineOpacity})`;
        for (let y = 0; y < canvas.height; y += 3) {
          ctx.fillRect(0, y, canvas.width, 1);
        }
      }

      animFrameRef.current = requestAnimationFrame(draw);
    };

    animFrameRef.current = requestAnimationFrame(draw);

    return () => {
      cancelAnimationFrame(animFrameRef.current);
      window.removeEventListener('resize', resize);
    };
  }, [config.noiseLevel, config.scanlineOpacity]);

  // CSS 变量注入
  useEffect(() => {
    const root = document.documentElement;
    root.style.setProperty('--atmos-bg', config.backgroundColor);
    root.style.setProperty('--atmos-tint', config.uiTint);
    root.style.setProperty('--atmos-glow', config.textGlowColor);
    root.style.setProperty('--atmos-transition', `${config.transitionMs}ms`);
    root.style.setProperty('--atmos-vignette', String(config.vignetteIntensity));
    document.body.style.backgroundColor = config.backgroundColor;
    document.body.style.transition = `background-color ${config.transitionMs}ms ease-in-out`;
  }, [config]);

  return (
    <AtmosphereContext.Provider value={config}>
      <canvas
        ref={canvasRef}
        style={{
          position: 'fixed',
          top: 0,
          left: 0,
          width: '100vw',
          height: '100vh',
          pointerEvents: 'none',
          zIndex: 9999,
          opacity: 0.8,
          transition: `opacity ${config.transitionMs}ms ease-in-out`,
        }}
      />
      {/* 暗角效果 */}
      <div
        style={{
          position: 'fixed',
          top: 0,
          left: 0,
          width: '100vw',
          height: '100vh',
          pointerEvents: 'none',
          zIndex: 9998,
          boxShadow: `inset 0 0 ${config.vignetteIntensity * 150}px rgba(0,0,0,${config.vignetteIntensity})`,
          transition: `box-shadow ${config.transitionMs}ms ease-in-out`,
        }}
      />
      {children}
    </AtmosphereContext.Provider>
  );
};