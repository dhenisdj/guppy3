# Tests generated by: guppy.gsl.Tester
# Date: Sat Aug 17 02:38:12 2019
class Tester:
    tests = {}
    def get_ex_1(self):
        class C:
            pass
        return C
    def get_ex_2(self):
        class C:
            pass
        return C
    def get_ex_3(self):
        import sys
        return id(sys)
    def get_ex_4(self):
        class C:
         pass
        return self.hp.Clodo(dictof=C)
    def get_ex_5(self):
        class C:
            pass
        return C
    def get_ex_6(self):
        import os
        return os.path.join(os.path.dirname(__file__),'profileexample.hpy')
    def get_ex_7(self):
        import os
        return open(os.path.join(os.path.dirname(__file__),'profileexample.hpy'))
    def get_ex_8(self):
        import os
        return os.path.join(os.path.dirname(__file__),'profileexample.hpy')
    def test_Use(self, arg):
        t0 = arg.Type(int)
        t1 = arg.Type(self.get_ex_5())
        t2 = arg.Size(0)
        t3 = arg.Root
        t4 = arg.Rcs()
        t5 = arg.Rcs(self.get_ex_4())
        t6 = arg.Rcs(self.hp.Clodo.sokind(int)(dictof=()))
        t7 = arg.Module(at=self.get_ex_3())
        t8 = arg.Module(name='sys', at=self.get_ex_3())
        t9 = arg.Unity()
        t10 = arg.Clodo(int)
        t11 = arg.Clodo(self.get_ex_1())
        t12 = arg.Clodo(dictof=())
        t13 = arg.Clodo(dictof=self.get_ex_2())
        t14 = arg.Id(id(None))
        t15 = arg.Module()
        t16 = arg.Module(name='sys')
        t17 = arg.Nothing
        t18 = arg.Anything
        t19 = arg.heapu()
        t20 = arg.load(self.get_ex_6())
        t21 = arg.load(self.get_ex_7())
        t22 = arg.load(self.get_ex_6(), use_readline=True)
        t23 = arg.load(self.get_ex_7(), use_readline=True)
        t24 = arg.heap()
        t25 = arg.heapg()
        t26 = arg.idset([1])
        t27 = arg.iso()
        t28 = arg.iso(())
        t29 = arg.findex()
        t30 = t29(0)
        t31 = arg.findex(self.hp.Anything)
        t32 = t31(0)
        t33 = arg.Via()
        t34 = arg.Via('[0]')
        t35 = arg.Via('.a')
        t36 = arg.doc
        t37 = arg.monitor()
        t38 = arg.pb()
        t39 = arg.pb(self.get_ex_8())
        t40 = arg.setref()
        t41 = arg.setrelheap()
        t42 = arg.setrelheap(self.hp.Nothing)
        t43 = arg.setrelheapg()
        t44 = arg.setrelheapg(self.hp.Nothing)
        t45 = arg.setrelheapu()
        t46 = arg.setrelheapu(self.hp.Nothing)
    tests['.tgt.heapykinds.Use'] = test_Use
