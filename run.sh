#! /bin/bash

pushd build > /dev/null

./QuoteFetch $@

popd >/dev/null