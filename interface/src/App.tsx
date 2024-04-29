import { ThemeProvider } from '@/components/theme-provider'
import { ModeToggle } from '@/components/mode-toggle'
import Landing from './pages/Landing'

import { motion } from 'framer-motion'
import { AuroraBackground } from './components/ui/aurora-background'

import './App.css'

function App() {
	return (
		<ThemeProvider defaultTheme="dark" storageKey="vite-ui-theme">
			<AuroraBackground>
				{/* <motion.div
					initial={{ opacity: 0.0, y: 40 }}
					whileInView={{ opacity: 1, y: 0 }}
					transition={{
						delay: 0.3,
						duration: 0.8,
						ease: 'easeInOut',
					}}
					className="relative flex flex-col items-center justify-center gap-4 px-4"> */}
					<Landing />
				{/* </motion.div> */}
			</AuroraBackground>

			<div className="fixed right-10 top-10">
				<ModeToggle />
			</div>
		</ThemeProvider>
	)
}

export default App
