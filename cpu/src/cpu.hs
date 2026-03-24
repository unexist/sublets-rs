--
-- @package sublets-rs
--
-- @file CPU plugin functions
-- @copyright (c) 2026-present Christoph Kappel <christoph@unexist.dev>
-- @version $Id$
--
-- This program can be distributed under the terms of the GNU GPLv3.
-- See the file LICENSE for details.
--

{-# LANGUAGE DeriveDataTypeable #-}

module Cpu where

--import Extism.PDK.Memory
--import Extism.PDK
import Data.Int

foreign import ccall "get_cpu" getCpu :: IO Int32

run :: IO Int32
run = do
    resOffset <- getCpu

    return 0

foreign export ccall "run" run :: IO Int32
