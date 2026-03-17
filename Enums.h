#ifndef ENUMS_H
#define ENUMS_H

// ==================== RESERVATION STATUS ====================
// Enumeration for reservation lifecycle states
enum ReservationStatus {
    PENDING,
    CONFIRMED,
    SEATED,
    COMPLETED,
    CANCELLED
};

// ==================== ORDER STATUS ====================
// Enumeration for order lifecycle states
enum OrderStatus {
    ORDER_PENDING,
    ORDER_PREPARING,
    ORDER_READY,
    ORDER_SERVED,
    ORDER_CANCELLED
};

#endif

