#include <Diagnostics/DiagnosticsBus.h>
#include <Diagnostics/BufferedDiagnostics.h>
#include <gtest/gtest.h>

TEST(DiagnosticsBus, DispatchToBufferedSink) {
    DiagnosticsBus bus;
    BufferedDiagnostics buf(5);
    bus.addSink(&buf);
    bus.info("hello");
    ASSERT_EQ(buf.history().size(), 1u);
    EXPECT_NE(buf.history()[0].find("hello"), std::string::npos);
}
