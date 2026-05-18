import { Star, createEmptyStar } from "./Star";

// Simple seeded random number generator
function createSeededRNG(seed: number) {
  let m_w = seed;
  let m_z = 987654321;
  const mask = 0xffffffff;

  return function() {
    m_z = (36969 * (m_z & 65535) + (m_z >> 16)) & mask;
    m_w = (18000 * (m_w & 65535) + (m_w >> 16)) & mask;
    let result = ((m_z << 16) + (m_w & 65535)) >>> 0;
    result /= 4294967296;
    return result;
  };
}

export function generateStars(seed: number, startIndex: number, count: number, 
  resourceRange: [number, number], popRange: [number, number], prefix: string): Star[] {
  const rng = createSeededRNG(seed);
  const stars: Star[] = [];
  for (let i = 0; i < count; i++) {
    const star = createEmptyStar(startIndex + i);
    // Name e.g. "GLX-A1", "GLX-B1"
    star.name = `${prefix}-${String.fromCharCode(65 + (i % 26))}${Math.floor(i/26) || ''}`;
    star.totalResource = Math.floor(rng() * (resourceRange[1] - resourceRange[0])) + resourceRange[0];
    star.currentResource = star.totalResource;
    star.populationLimit = Math.floor(rng() * (popRange[1] - popRange[0])) + popRange[0];
    stars.push(star);
  }
  return stars;
}
