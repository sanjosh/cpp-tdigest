#include "../tdigest/tdigest.hpp"

#include <gtest/gtest.h>

TEST(TDigestTest, LoadSave) {
    TDigest* tdigest = new TDigest(100);

    for (int i = 0; i < 1000; i++) 
    {
      tdigest->add(rand() % 1001);
    }

    std::string str;
    tdigest->save(str);

    TDigest* new_digest = new TDigest(100);
    new_digest->load(str);

    EXPECT_EQ(new_digest->quantile(0.5), tdigest->quantile(0.5));
}

