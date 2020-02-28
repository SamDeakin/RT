#pragma once

namespace Core {

    /**
     * A common abstract class for input events that may be received from the user.
     * Used by both Window implementations and App implementations, to keep them in sync.
     */
    class InputReceiver {
    public:
        InputReceiver() = default;
        virtual ~InputReceiver() = default;

        // -- GLFW Events --
        // The subclass should implement these to receive event details.

        /**
         * Indicates that the window and viewport have been resized.
         * Both values being zero would mean that the window has been minimized.
         * @param width: The new viewport width
         * @param height: The new viewport height
         * @return true if the event was handled
         */
        virtual bool windowResized(int width, int height);

        // -- End GLFW Events --
    };
}
