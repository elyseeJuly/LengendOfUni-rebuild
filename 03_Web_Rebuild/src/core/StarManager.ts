import { Star, createEmptyStar } from "./Star";
import starsData from "../data/stars.json";
import { StarArea } from "../types/enums";

export class StarManager {
  public stars: Map<number, Star> = new Map();

  constructor() {
    this.init();
  }

  public init(): void {
    starsData.forEach((data: any) => {
      const star = createEmptyStar(data.Index);
      star.name = data.Name;
      star.isPlanet = data.IsPlanet === 1;
      star.totalResource = data.Resource || 0;
      star.currentResource = star.totalResource;
      
      // Random generation bounds mapping logic goes here if needed
      
      this.stars.set(star.index, star);
    });

    // BUG-01 Fix: Initialize Earth
    const earth = this.stars.get(4);
    if (earth) {
      earth.belongToCivi = "地球";
      earth.found = true;
      earth.populationLimit = 1000;
      earth.currentPopulation = 100;
    }
  }

  public getStar(index: number): Star | undefined {
    return this.stars.get(index);
  }

  public getAllStars(): Star[] {
    return Array.from(this.stars.values());
  }

  public getStarsByArea(area: StarArea): Star[] {
    // 太阳系 0-8
    // 50光年 9-100
    // 1万光年 101-200
    // 银河系 201-1000
    return this.getAllStars().filter(s => {
      if (area === StarArea.SOLARSYSTEM) return s.index <= 8;
      if (area === StarArea.LIGHTYEAR_50) return s.index > 8 && s.index <= 100;
      if (area === StarArea.LIGHTYEAR_1W) return s.index > 100 && s.index <= 200;
      if (area === StarArea.GALAXY) return s.index > 200 && s.index <= 1000;
      return false;
    });
  }
}
