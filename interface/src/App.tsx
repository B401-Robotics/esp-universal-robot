import { ThemeProvider } from '@/components/theme-provider'
import { ModeToggle } from '@/components/mode-toggle'
import { Toaster } from '@/components/ui/toaster'
import Landing from './pages/Landing'

import { motion } from 'framer-motion'
import { AuroraBackground } from './components/ui/aurora-background'

import './App.css'

function App() {
	return (
		<ThemeProvider defaultTheme="dark" storageKey="vite-ui-theme">
			<AuroraBackground>
				<motion.div
					initial={{ opacity: 0.0, y: 40 }}
					whileInView={{ opacity: 1, y: 0 }}
					transition={{
						delay: 0.3,
						duration: 0.8,
						ease: 'easeInOut',
					}}
					className="relative flex flex-col items-center justify-center gap-4 px-4">
					<Landing />
				</motion.div>
			</AuroraBackground>

			<div className="fixed right-8 top-8">
				<ModeToggle />
			</div>
			{/* footer */}
			<div
				className="fixed bottom-10 flex w-full flex-col items-center gap-3 text-sm
			text-muted-foreground sm:flex-row sm:justify-end sm:pr-10
			">
				<p>© 2024 B401 Robotics. </p>
				<p>Build with shadcn/ui + Aceternity UI</p>
			</div>
			<Toaster />
		</ThemeProvider>
	)
}

export default App
