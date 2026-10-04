CREATE TABLE devices (
    device_id INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE
);

CREATE TABLE telemetry (
    device_id INTEGER NOT NULL,
    sequence_no INTEGER NOT NULL,
    measured_at TEXT NOT NULL,
    ingested_at TEXT NOT NULL,
    quality TEXT NOT NULL,
    value REAL NOT NULL,
    PRIMARY KEY (device_id, sequence_no),
    FOREIGN KEY (device_id) REFERENCES devices(device_id)
);

CREATE INDEX telemetry_device_time_idx
    ON telemetry(device_id, measured_at);

CREATE TABLE alarms (
    alarm_id INTEGER PRIMARY KEY,
    device_id INTEGER NOT NULL,
    severity INTEGER NOT NULL,
    occurred_at TEXT NOT NULL,
    acknowledged_at TEXT,
    cleared_at TEXT,
    message TEXT NOT NULL,
    FOREIGN KEY (device_id) REFERENCES devices(device_id)
);
