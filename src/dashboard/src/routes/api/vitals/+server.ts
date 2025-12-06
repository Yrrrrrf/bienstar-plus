import { json } from "@sveltejs/kit";
import type { RequestHandler } from "./$types";

// In-memory store
interface VitalReading {
  hr: number;
  spo2: number;
  temp: number;
  status: string;
  timestamp: string;
}

let currentVitals: VitalReading = {
  hr: 0,
  spo2: 0,
  temp: 0,
  status: "unknown",
  timestamp: new Date().toISOString(),
};

let history: VitalReading[] = [];

export const GET: RequestHandler = async () => {
  return json({
    current: currentVitals,
    history: history,
  });
};

export const POST: RequestHandler = async ({ request }) => {
  try {
    const data = await request.json();

    // Basic validation
    if (
      typeof data.hr !== "number" || typeof data.spo2 !== "number" ||
      typeof data.temp !== "number"
    ) {
      return json({ error: "Invalid data format" }, { status: 400 });
    }

    const reading: VitalReading = {
      hr: data.hr,
      spo2: data.spo2,
      temp: data.temp,
      status: data.status || "normal",
      timestamp: data.timestamp || new Date().toISOString(),
    };

    currentVitals = reading;
    history.unshift(reading); // Add to beginning
    if (history.length > 50) {
      history.pop(); // Keep max 50
    }

    return json({ success: true, reading });
  } catch (err) {
    return json({ error: "Failed to parse JSON" }, { status: 400 });
  }
};
