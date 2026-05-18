import { Person, createEmptyPerson } from "./Person";
import personsData from "../data/persons.json";

export class PersonManager {
  public persons: Map<string, Person> = new Map();
  public availablePersons: Set<string> = new Set();
  
  constructor() {
    this.init();
  }

  public init(): void {
    personsData.forEach((data: any) => {
      const p = createEmptyPerson(data.name || data.Name);
      p.faceFile = data.faceFile || data.FaceFile || "";
      p.treachery = data.treachery ?? data.Treachery ?? 0;
      p.science = data.science ?? data.Science ?? 0;
      p.art = data.art ?? data.Art ?? 0;
      p.economy = data.economy ?? data.Economy ?? 0;
      p.army = data.army ?? data.Army ?? 0;
      p.leadership = data.leadership ?? data.Leadership ?? 0;
      p.social = data.social ?? data.Social ?? 0;
      
      const initialWhitelist = ["丁仪", "汪淼", "常伟思", "大史", "雷志成", "杨卫宁", "叶文洁"];
      this.persons.set(p.name, p);
      if (initialWhitelist.includes(p.name)) {
        this.availablePersons.add(p.name);
      }
    });
  }

  public unlockPerson(name: string): void {
    if (this.persons.has(name) && !this.availablePersons.has(name)) {
      this.availablePersons.add(name);
      // Let Game instance add a history log or toast if possible, handled externally
    }
  }

  public getPerson(name: string): Person | undefined {
    return this.persons.get(name);
  }

  public getAllPersons(): Person[] {
    return Array.from(this.persons.values());
  }
}
